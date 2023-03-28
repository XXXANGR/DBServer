#include <stdio.h>
#include <string.h>

#include "extclib/http.h"

void index_page(int conn, HTTPreq *req);
void about_page(int conn, HTTPreq *req);
void index_img(int conn, HTTPreq *req);
void about_img(int conn, HTTPreq *req);

int main(void)
{
	HTTP *serve = new_http("127.0.0.1:7545");

	handle_http(serve, "/", index_page);
	handle_http(serve, "/about/", about_page);
	handle_http(serve, "/index_img.jpg", index_img);
	handle_http(serve, "/about/about_img.jpg", about_img);
	
	listen_http(serve);
	free_http(serve);
	return 0;
}

void index_page(int conn, HTTPreq *req) {
	if(strcmp(req->path, "/") != 0) {
		parse_http(conn, "page404.html", HTTP_TEXT, NULL);
		return;
	}
	parse_http(conn, "index.html", HTTP_TEXT_TABLE, "price");
}

void about_page(int conn, HTTPreq *req) {
	if(strcmp(req->path, "/about/") != 0) {
		parse_http(conn, "page404.html", HTTP_TEXT, NULL);
		return;
	}
	parse_http(conn, "about.html", HTTP_TEXT, NULL);
}

void index_img(int conn, HTTPreq *req)
{
	if (strcmp(req->path, "/index_img.jpg") != 0)
	{
		parse_http(conn, "page404.html", HTTP_TEXT, NULL);
		return;
	}
	parse_http(conn, "index_img.jpg", HTTP_IMG_JPG, NULL);
}

void about_img(int conn, HTTPreq *req)
{
	if (strcmp(req->path, "/about/about_img.jpg") != 0)
	{
		parse_http(conn, "page404.html", HTTP_TEXT, NULL);
		return;
	}
	parse_http(conn, "about_img.jpg", HTTP_IMG_JPG, NULL);
}
