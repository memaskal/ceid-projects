function [ yht ] = dpcm_decoder(a_q, yh)

    N = length(yh);
    p = length(a_q);
    
    yht = zeros(N, 1);
    % Calculate first p values
    yht(1:p) = yh(1:p);
    
    % Calculate next p+1..N values
    for i = p+1:N
        % Prediction y'
        yt = a_q' * yht(i-1:-1:i-p);
        % Ouput yhat'
        yht(i) = yt + yh(i);
    end
end

