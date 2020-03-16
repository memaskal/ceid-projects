function [ a_q, yh, y ] = dpcm_encoder(x, p, qBits, qMin, qMax)

    N = length(x);
    r = zeros(p, 1);
    R = zeros(p, p);
    
    % Populate R matrix and vector r
    for i = 1:p
        r(i) = 1/(N - p) * (x(p+1:N)' * x(p+1-i:N-i));
        for j = 1:p
            R(i, j) =  1/(N - p) *(x(p+1-j:N-j)' * x(p+1-i:N-i));
        end
    end
       
    % Quantize a with N=8 bits and [-2,2] dyanmic range
    a_q = my_quantizer(R\r, 8, -2, 2);
    
    y = zeros(N, 1);
    yh = zeros(N, 1);
    
    % Calculate first p values
    y(1:p) = x(1:p);
    yh(1:p) = my_quantizer(y(1:p), qBits, qMin, qMax);
    
    % Reverse order, length = p
    yht = yh(p:-1:1);
    
    % Calculate next p+1..N values
    for i = p+1:N
        % Prediction y'
        yt = a_q' * yht;
        % Prediction error y
        y(i) = x(i) - yt;
        % Ouput yhat
        yh(i) = my_quantizer(y(i), qBits, qMin, qMax);
        % Push to stack, shift right
        yht = [yt + yh(i); yht(1:p-1)];
    end
end

