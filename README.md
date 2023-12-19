## Source Code
### metric
Prometheus Four Metric Type Class
1. Counter
2. Guage
3. Histogram
4. Summary

### http
[CivetWeb](https://github.com/civetweb/civetweb) HTTP Server

## BenchMark
1000 Counter + 1000 Guage + 1000 Histogram

Every Scrape to `/metrics` avg cost nearly `25ms`


## Usage
Refer to `src/main.cc`
