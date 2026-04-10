/*
 * hlquery C++ Client - HTTP Request Handler Implementation
 *
 * Copyright (C) 2021-2026, Carlos F. Ferry <carlos.ferry@gmail.com>
 *
 * This file is part of hlquery, released under the BSD License version 3.
 */

#include <arpa/inet.h>
#include <cstring>
#include <errno.h>
#include <iomanip>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <regex>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include "hlquery/request.h"
#include "hlquery/utils/Url.h"

#ifdef HLQUERY_HAS_OPENSSL
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

namespace hlquery
{

Request::Request(const std::string& base_url, int timeout,
                 const std::string& auth_token, const std::string& auth_method,
                 bool tls_verify)
    : timeout_(timeout),
      auth_token(auth_token),
      auth_method_(auth_method),
      tls_verify_(tls_verify)
{
     /* Remove trailing slash */

     std::string normalized = base_url;
     if (!normalized.empty() && normalized.back() == '/')
     {
          normalized.pop_back();
     }
     this->base_url = normalized;
}

void Request::setAuthToken(const std::string& token, const std::string& method)
{
     auth_token = token;
     auth_method_ = method;
}

void Request::clearAuth()
{
     auth_token.clear();
}

std::string Request::buildUrl(const std::string& path, const std::map<std::string, std::string>& query_params) const
{
     std::string url = base_url + path;

     if (!query_params.empty())
     {
          url += "?";
          bool first = true;
          for (const auto& param : query_params)
          {
               if (!first)
                    url += "&";
               url += utils::urlEncode(param.first) + "=" + utils::urlEncode(param.second);
               first = false;
          }
     }

     return url;
}

void Request::parseUrl(const std::string& url, std::string& host, int& port, bool& use_ssl) const
{
     use_ssl = (url.find("https://") == 0);
     std::string protocol = use_ssl ? "https://" : "http://";

     std::string url_no_protocol = url;
     if (url.find(protocol) == 0)
     {
          url_no_protocol = url.substr(protocol.length());
     }

     size_t colon_pos = url_no_protocol.find(':');
     size_t slash_pos = url_no_protocol.find('/');

     if (colon_pos != std::string::npos && (slash_pos == std::string::npos || colon_pos < slash_pos))
     {
          host = url_no_protocol.substr(0, colon_pos);
          std::string port_str = url_no_protocol.substr(colon_pos + 1,
                                                        (slash_pos != std::string::npos ? slash_pos - colon_pos - 1 : std::string::npos));
          port = std::stoi(port_str);
     }
     else
     {
          host = (slash_pos != std::string::npos) ? url_no_protocol.substr(0, slash_pos) : url_no_protocol;
          port = use_ssl ? 443 : 80;
     }
}

Response Request::makeHttpRequest(const std::string& method, const std::string& url, const std::string& body) const
{
     std::string host;
     int port;
     bool use_ssl;
     parseUrl(url, host, port, use_ssl);

     std::string path = url;
     if (url.find("://") != std::string::npos)
     {
          size_t path_start = url.find('/', url.find("://") + 3);
          if (path_start != std::string::npos)
          {
               path = url.substr(path_start);
          }
          else
          {
               path = "/";
          }
     }

     Response response(-1, nlohmann::json::object());

#ifdef HLQUERY_HAS_OPENSSL
     SSL_CTX* ssl_ctx = nullptr;
     SSL* ssl = nullptr;
#endif

     int sock = socket(AF_INET, SOCK_STREAM, 0);
     if (sock < 0)
     {
          throw RequestException("Failed to create socket: " + std::string(strerror(errno)));
     }

     /* Set socket timeout */

     struct timeval timeout;
     timeout.tv_sec = timeout_;
     timeout.tv_usec = 0;
     setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
     setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

     /* Resolve hostname */

     struct hostent* server = gethostbyname(host.c_str());
     if (server == nullptr)
     {
          close(sock);
          throw RequestException("Failed to resolve hostname '" + host + "': " + std::string(hstrerror(h_errno)));
     }

     /* Setup address */

     struct sockaddr_in serv_addr;
     memset(&serv_addr, 0, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
     serv_addr.sin_port = htons(port);

     /* Connect */

     if (connect(sock, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0)
     {
          close(sock);
          throw RequestException("Failed to connect to " + host + ":" + std::to_string(port) + ": " + std::string(strerror(errno)));
     }

#ifdef HLQUERY_HAS_OPENSSL
     if (use_ssl)
     {
          SSL_library_init();
          SSL_load_error_strings();
          OpenSSL_add_all_algorithms();

          ssl_ctx = SSL_CTX_new(TLS_client_method());
          if (!ssl_ctx)
          {
               close(sock);
               throw RequestException("Failed to create SSL context");
          }
          if (tls_verify_)
          {
               SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, nullptr);
               if (SSL_CTX_set_default_verify_paths(ssl_ctx) != 1)
               {
                    SSL_CTX_free(ssl_ctx);
                    close(sock);
                    throw RequestException("Failed to load system CA certificates");
               }
          }
          else
          {
               SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, nullptr);
          }

          ssl = SSL_new(ssl_ctx);
          if (!ssl)
          {
               SSL_CTX_free(ssl_ctx);
               close(sock);
               throw RequestException("Failed to create SSL connection");
          }

          if (tls_verify_)
          {
               X509_VERIFY_PARAM* verify_params = SSL_get0_param(ssl);
               if (verify_params)
               {
                    X509_VERIFY_PARAM_set1_host(verify_params, host.c_str(), 0);
               }
          }

          SSL_set_fd(ssl, sock);
          if (SSL_connect(ssl) != 1)
          {
               SSL_free(ssl);
               SSL_CTX_free(ssl_ctx);
               close(sock);
               throw RequestException("Failed to establish SSL connection");
          }
     }
#endif

     /* Build HTTP request */

     std::stringstream request;
     request << method << " " << path << " HTTP/1.1\r\n";
     request << "Host: " << host << ":" << port << "\r\n";
     request << "User-Agent: hlquery-cpp-client/1.0\r\n";
     request << "Accept: application/json\r\n";
     request << "Connection: close\r\n";

     if (!auth_token.empty())
     {
          if (auth_method_ == "api-key")
          {
               request << "X-API-Key: " << auth_token << "\r\n";
          }
          else
          {
               request << "Authorization: Bearer " << auth_token << "\r\n";
          }
     }

     if (!body.empty())
     {
          request << "Content-Type: application/json\r\n";
          request << "Content-Length: " << body.length() << "\r\n";
     }

     request << "\r\n";
     if (!body.empty())
     {
          request << body;
     }

     std::string request_str = request.str();

     /* Send request */

     int sent = 0;
     if (use_ssl)
     {
#ifdef HLQUERY_HAS_OPENSSL
          sent = SSL_write(ssl, request_str.c_str(), request_str.length());
#else
          close(sock);
          throw RequestException("SSL not available but HTTPS URL requested");
#endif
     }
     else
     {
          sent = send(sock, request_str.c_str(), request_str.length(), 0);
     }

     if (sent < 0)
     {
#ifdef HLQUERY_HAS_OPENSSL
          if (use_ssl)
          {
               SSL_free(ssl);
               SSL_CTX_free(ssl_ctx);
          }
#endif
          close(sock);
          throw RequestException("Failed to send HTTP request: " + std::string(strerror(errno)));
     }

     /* Read response */

     char buffer[4096];
     std::string response_str;
     int bytes_received;

     while (true)
     {
          if (use_ssl)
          {
#ifdef HLQUERY_HAS_OPENSSL
               bytes_received = SSL_read(ssl, buffer, sizeof(buffer) - 1);
#else
               bytes_received = 0;
#endif
          }
          else
          {
               bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
          }

          if (bytes_received <= 0)
               break;

          buffer[bytes_received] = '\0';
          response_str += buffer;

          /* Check if we have received the complete HTTP response */

          if (response_str.find("\r\n\r\n") != std::string::npos)
          {
               size_t content_length_pos = response_str.find("Content-Length: ");
               if (content_length_pos != std::string::npos)
               {
                    size_t header_end = response_str.find("\r\n\r\n");
                    size_t content_start = header_end + 4;
                    size_t content_length_end = response_str.find("\r\n", content_length_pos);
                    int content_length = std::stoi(response_str.substr(content_length_pos + 16,
                                                                       content_length_end - content_length_pos - 16));

                    if (response_str.length() - content_start >= static_cast<size_t>(content_length))
                    {
                         break;
                    }
               }
               else
               {
                    break;
               }
          }
     }

#ifdef HLQUERY_HAS_OPENSSL
     if (use_ssl)
     {
          SSL_shutdown(ssl);
          SSL_free(ssl);
          SSL_CTX_free(ssl_ctx);
     }
#endif
     close(sock);

     /* Parse response */

     size_t header_end = response_str.find("\r\n\r\n");
     if (header_end == std::string::npos)
     {
          throw RequestException("Invalid HTTP response: missing headers");
     }

     std::string headers_str = response_str.substr(0, header_end);
     std::string body_str = response_str.substr(header_end + 4);

     /* Parse status line */

     size_t first_line_end = headers_str.find("\r\n");
     std::string status_line = headers_str.substr(0, first_line_end);

     std::istringstream status_stream(status_line);
     std::string http_version, status_code_str, status_text;
     status_stream >> http_version >> status_code_str >> status_text;

     int status_code = std::stoi(status_code_str);

     /* Parse headers */

     std::map<std::string, std::string> headers;
     std::istringstream header_stream(headers_str.substr(first_line_end + 2));
     std::string header_line;

     while (std::getline(header_stream, header_line) && !header_line.empty())
     {
          if (header_line.back() == '\r')
               header_line.pop_back();
          size_t colon_pos = header_line.find(':');
          if (colon_pos != std::string::npos)
          {
               std::string key = header_line.substr(0, colon_pos);
               std::string value = header_line.substr(colon_pos + 1);
               /* Trim whitespace */

               value.erase(0, value.find_first_not_of(" \t"));
               value.erase(value.find_last_not_of(" \t") + 1);
               headers[key] = value;
          }
     }

     /* Check for authentication errors */

     if (status_code == 403 && !body_str.empty())
     {
          try
          {
               nlohmann::json error_json = nlohmann::json::parse(body_str);
               std::string error_msg = error_json.value("error", "");
               std::string message = error_json.value("message", "");

               if (error_msg.find("Authentication is disabled") != std::string::npos ||
                   message.find("Tokens are not accepted when authentication is disabled") != std::string::npos)
               {
                    throw AuthenticationException(
                         "Authentication is disabled on the server. Remove the token from your client configuration. "
                         "Server message: " +
                         (message.empty() ? error_msg : message));
               }
          }
          catch (const nlohmann::json::parse_error&)
          {
               /* Not JSON, continue */
          }
          catch (const AuthenticationException&)
          {
               throw;
          }
     }

     return Response(status_code, body_str, headers);
}

Response Request::execute(const std::string& method, const std::string& path,
                          const nlohmann::json& body, const std::map<std::string, std::string>& query_params)
{
     std::string url = buildUrl(path, query_params);
     std::string body_str = body.is_null() ? "" : body.dump();
     return makeHttpRequest(method, url, body_str);
}

}
