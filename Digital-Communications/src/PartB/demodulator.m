function [ r ] = demodulator(rt, T_symbol, T_c, E_s)

    [symbols, samples] = size(rt);
    y = zeros(samples, 2);
    r = zeros(symbols, 2);
    
    % Calculate gT(t)
    gt = sqrt(2 * E_s/T_symbol);
    
    % Multiplication constants
    for t = 1:samples
        y(t, 1) = gt * cos(2*pi*t/T_c);
        y(t, 2) = gt * sin(2*pi*t/T_c);
    end
    
    % Calculate position r(i)
    for i = 1:symbols
        r(i,:) = [rt(i,:)*y(:,1), rt(i,:)*y(:,2)];
    end   
end

