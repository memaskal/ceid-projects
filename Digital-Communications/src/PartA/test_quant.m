size = 5;
Max  = 4;
Min  =-4;
N = 2;
y = Min + (Max-Min).*rand(size,1)
my_quantizer(y, N, Min, Max)
%[yh, c] = my_quantizer2(y, N, Min, Max);
%c(yh)