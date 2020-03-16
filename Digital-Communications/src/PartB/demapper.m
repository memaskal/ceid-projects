function [ x ] = demapper(smh, M, isGray)
    
    b = ceil(log2(M));
    
    % Decode from gray to dec
    if (isGray == 1) 
        smh = gray2bin(smh, 'psk', M);
    end
    % Serialize dec numbers as a bit vector
    x = reshape(de2bi(smh, b, 2, 'left-msb')', [], 1);
end
