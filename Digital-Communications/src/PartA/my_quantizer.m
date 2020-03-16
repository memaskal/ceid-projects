function [ y_h ] = my_quantizer(y, N, min_value, max_value)

    % Calculate zone's range
    delta = abs(max_value - min_value) / 2^N;
    
    % Cut off edge values
    if y > max_value
        y = max_value;
    elseif y < min_value
        y = min_value;
    end
    
    % Calculate quantized y
    y_h = delta * (floor(y/delta) + 0.5);
end