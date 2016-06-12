What's qDecoder?
================

qDecoder is a CGI library for C/C++ programming language which has been developed since 1996.

## Features

  * Supports parsing a request encoded by GET/POST method
  * Supports parsing multipart/form-data encoding. (in-memory and direct disk)
  * Supports COOKIE handling.
  * Supports Session management.
  * Supports FastCGI

## API Reference

[API Reference](http://wolkykim.github.io/qdecoder/doc/html/files.html)

## How easy making CGI applications in C/C++

```
  [HTML sample]
  <form action="your_program.cgi">
    <input type="text" name="color">
    <input type="submit">
  </form>

  [Your Code]
  qentry_t *req = qcgireq_parse(NULL, 0);

  qcgires_setcontenttype(req, "text/plain");
  const char *color = req->getstr(req, "color", false);
  if (color != NULL) {
    printf("color = %s\n", color);
  }

  req->free(req);
```

The order of parsing sequence is (1)COOKIE (2)POST (3)GET. Thus if there is a same query name existing in different methods, COOKIE values will be stored first than POST, GET values will be added at the very last into a qentry linked-list.

Below is an example to parse only two given methods. Please note that when multiple methods are specified, it'll be parsed in the order of COOKIE, POST and GET.

```
  qentry_t *req = qcgireq_parse(req, Q_CGI_COOKIE | Q_CGI_POST);
```

To change the order of parsing sequence, you can call qcgireq_parse() multiple times in the order that you want as below.

```
  qentry_t *req;
  req = qcgireq_parse(req, Q_CGI_POST);
  req = qcgireq_parse(req, Q_CGI_GET);
  req = qcgireq_parse(req, Q_CGI_COOKIE);
```

In terms of multipart/form-data encoding(used for file uploading), qDecoder can handle that in two different ways internally.

* default mode : Uploading file will be processed only in memory. (see examples/upload.c) 
* file mode : Uploading file will be stored directly into disk. (see examples/uploadfile.c)

You can switch to file mode by calling qCgiRequestSetOption(). 

```
  Q_ENTRY *req = qcgireq_setoption(NULL, true, "/tmp", 86400);
  req = qcgireq_parse(req, 0);
  // ...your codes here...
  req->free(req);
```

Basically, when file is uploaded qDecoder store it's meta information like below. 

  * (VARIABLE_NAME) - In the default mode, this is binary data. In the file mode this value is same as "(VARIABLE_NAME).savepath". 
  * (VARIABLE_NAME).filename - File name itself, path information will be removed. 
  * (VARIABLE_NAME).length - File size, the number of bytes. 
  * (VARIABLE_NAME).contenttype - Mime type like 'text/plain'. 
  * (VARIABLE_NAME).savepath - Only appended only in file mode. The file path where the uploaded file is saved.

```
  [default mode example]
  binary = (...binary data...)
  binary.filename = hello.xls
  binary.length = 3292
  binary.contenttype = application/vnd.ms-excel

  [file mode example]
  binary = tmp/q_wcktIq
  binary.length = 60014
  binary.filename = hello.xls
  binary.contenttype = application/vnd.ms-excel
  binary.savepath = tmp/q_wcktIq
```

Please refer the examples included in the source package for more detailed samples.

## Contributors

The following people have helped with suggestions, ideas, code or fixing bugs:
(in alphabetical order by first name)

* [nyov](https://github.com/nyov)
* Seungyoung Kim

If we have forgotten your name or spelled not in the way you want, please let us know.

