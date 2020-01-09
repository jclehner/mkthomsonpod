#!/usr/bin/ruby

require 'openssl'
require 'date'

date = ARGV.length >= 1 ? DateTime.parse(ARGV[0]) : DateTime.now
msg = date.strftime("%Y.%m.%d")
key = ARGV.length >= 2 ? ARGV[1] : "thomson525"

mac = OpenSSL::HMAC.hexdigest("MD5", key, msg)
puts mac[0, 15]


