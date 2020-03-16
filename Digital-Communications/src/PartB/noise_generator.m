function [ rt ] = noise_generator(st, M, Es, SNR)
    
    % Calculate Eb and sigma
    Eb = Es/ceil(log2(M));
    sigma = sqrt(Eb/(2 * 10^(SNR/10)));
    
    [symbols, samples] = size(st);
    % For each sample create awgn noise
    noise = sigma * randn(symbols, samples);
    rt = st + noise;
end

