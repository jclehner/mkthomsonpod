mkthomsonpod
========================================================

This is a PoD (password of the day) generator for Thomson
and Technicolor cable modems. It has been verified to
work on a Thomson TWG850, TWG870 and Technicolor TC7200.

It takes one (or two) arguments, the first being a unix
timestamp of the modem's current time (usually indicated
somewhere in the web interface). The second argument
is an optional salt, that defaults to `"thomson"` on the
firmwares I've seen so far.

Requires OpenSSL or compatible library.

Usage:
```
$ ./mkthomsonpod 87318
cadff4300001088

$ ./mkthomsonpod 87318 thomson
cadff4300001088

$ ./mkthomsonpod 87318 foobar
ec69e90125bb926

$ ./mkthomsonpod `date +%s`
76206924ebddf1b
```


