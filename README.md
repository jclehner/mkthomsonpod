mkthomsonpod
========================================================

This is a PoD (password of the day) generator for Thomson and
Technicolor cable modems. It has been verified to work on a
Thomson TWG850, TWG870 and Technicolor TC7200.

The password is generated from the first 15 bytes of the hash.
```
HMAC_MD5(data="YYYY.mm.dd", key="thomson525")
```

####### Usage

Get PoD for current date
```
$ ./mkthomsonpod.rb 
8dd1ec1d039a4ab
```

Get PoD for specific date
```
$ ./mkthomsonpod.rb 1970-01-02
cadff4300001088
```

Get PoD for specific date, using custom key
```
$ ./mkthomsonpod.rb 1970-01-02 foobar
ec69e90125bb926
```


