# INSTALL_curl_Windows.md

Robin Rowe 2019/4/4

To build from sources...

	cd /code
	git clone https://github.com/madler/zlib.git
	git clone https://github.com/libssh2/libssh2.git
	git clone https://github.com/curl/curl.git
	
Copy relevant config.cmake files from libunistd. The libssh2 build needs zlib config. The curl build needs libssh2 config.