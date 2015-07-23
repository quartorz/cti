# cti
Compile Time Interval

[bcl](https://github.com/quartorz/bcl)の`bcl::encode`で生成される、double型の値をテンプレートで表現したものの組で区間を表現します  
[kv](http://verifiedby.me/kv)の`kv::rop<double>`をconstexpr化して`kv::interval<T>`の`inf`と`sup`をテンプレート引数で保持するように変更しただけです  
C++14以降でコンパイルする必要があります
