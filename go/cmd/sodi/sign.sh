#!/bin/sh

openssl dgst -sha256 -sign certs/key.pem -out $1