

void on_open(uv_fs_t *req) {
	if (req->result != -1) {
		uv_fs_read(uv_default_loop(), &read_req, req->result, buffer,
				sizeof(buffer), -1, on_read);
	} else {
		fprint(stderr, "error opening file %d\n", req->errorno);
	}
	uv_fs_req_cleanup(req);
}

void on_read(uv_fs_t *req) {
	uv_fs_req_cleanup(req);
	if (req->result < 0) {
		fprintf(stderr, "Read error %s\n", uv_strerror(uv_last_error(uv_default_loop())));
	} else if (req->result == 0) {
		uv_fs_t close_req;
		uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	} else {
		uv_fs_write(uv_default_loop(), &write_req, 1, buffer, req->result, -1, on_write);	
	}
}

void on_write(uv_fs_t *req) {
	uv_fs_req_cleanup(req);
	if (req->result < 0) {
		fprintf(stderr, "Write error");
	}
}





