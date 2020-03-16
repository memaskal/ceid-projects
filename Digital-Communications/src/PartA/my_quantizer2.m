function [xq,centers] = my_quantizer2(x,N,min_value,max_value) % ---------
% INPUT -----------------------------------------------------------------
%   x: vector of input signal
%   N: number of bits to be used
%   min_value: minimum acceptable value of input signal
%   max_value: maximum acceptable value of input signal
% OUTPUT ----------------------------------------------------------------
%   xq: vector of output signal
%   centers: centers of quantization segments
% -----------------------------------------------------------------------

% Number of bits used.
v = N;
% Levels of quantization.
quant_levels = 2^v;
% Initialization of vector xq.
xq = zeros(length(x),1);
% Quantization step ?.
quant_step = (abs(min_value)+max_value)/quant_levels;
% Calculation of centers.
centers = zeros(quant_levels,1); % Initialization.
for i=1:quant_levels
    centers(i) = max_value-(2*(i-1)+1)*(quant_step/2);
end
% Calculation of output signal. (quantization)
q_max_value = 1; q_min_value = quant_levels;
for i=1:length(x)
    % If x(i) is greater or equal to max_value.
    if (x(i) >= max_value), xq(i) = q_max_value;
    % If x(i) is smaller than min_value.
    elseif (x(i) < min_value), xq(i) = q_min_value;
    else % For every other case in between.
        for n=1:quant_levels
            if (x(i) >= (centers(n)-(quant_step/2)) && ...
                x(i) < (centers(n)+(quant_step/2)))
                xq(i) = n;
            end
        end
    end
end