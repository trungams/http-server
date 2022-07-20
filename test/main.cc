// Simple unit tests without using any framework

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <iterator>
#include <string>

#include "http_message.h"
#include "uri.h"

using namespace simple_http_server;

#define EXPECT_TRUE(x)                                                   \
  {                                                                      \
    if (!(x))                                                            \
      err++, std::cerr << __FUNCTION__ << " failed on line " << __LINE__ \
                       << std::endl;                                     \
  }

int err = 0;

void test_uri_path_to_lowercase() {
  std::string path = "/SayHello.HTML?name=abc&message=welcome";
  std::string lowercase_path;
  std::transform(path.begin(), path.end(), std::back_inserter(lowercase_path),
                 [](char c) { return tolower(c); });

  Uri uri("/SayHello.html?name=abc&message=welcome");
  EXPECT_TRUE(uri.path() == lowercase_path);
}

void test_method_to_string() {
  EXPECT_TRUE(to_string(HttpMethod::GET) == "GET");
}

void test_version_to_string() {
  EXPECT_TRUE(to_string(HttpVersion::HTTP_1_1) == "HTTP/1.1");
}

void test_status_code_to_string() {
  EXPECT_TRUE(to_string(HttpStatusCode::ImATeapot) == "I'm a Teapot");
  EXPECT_TRUE(to_string(HttpStatusCode::NoContent) == "");
}

void test_string_to_method() {
  EXPECT_TRUE(string_to_method("GET") == HttpMethod::GET);
  EXPECT_TRUE(string_to_method("post") == HttpMethod::POST);
}

void test_string_to_version() {
  EXPECT_TRUE(string_to_version("HTTP/1.1") == HttpVersion::HTTP_1_1);
}

void test_request_to_string() {
  HttpRequest request;
  request.SetMethod(HttpMethod::GET);
  request.SetUri(Uri("/"));
  request.SetHeader("Connection", "Keep-Alive");
  request.SetContent("hello, world\n");

  std::string expected_str;
  expected_str += "GET / HTTP/1.1\r\n";
  expected_str += "Connection: Keep-Alive\r\n";
  expected_str += "Content-Length: 13\r\n\r\n";
  expected_str += "hello, world\n";

  EXPECT_TRUE(to_string(request) == expected_str);
}

void test_response_to_string() {
  HttpResponse response;
  response.SetStatusCode(HttpStatusCode::InternalServerError);

  std::string expected_str;
  expected_str += "HTTP/1.1 500 Internal Server Error\r\n\r\n";

  EXPECT_TRUE(to_string(response) == expected_str);
}

int main(void) {
  std::cout << "Running tests..." << std::endl;

  test_uri_path_to_lowercase();
  test_method_to_string();
  test_version_to_string();
  test_status_code_to_string();
  test_string_to_method();
  test_string_to_version();
  test_request_to_string();
  test_response_to_string();

  std::cout << "All tests have finished. There were " << err
            << " errors in total" << std::endl;
  return 0;
}
