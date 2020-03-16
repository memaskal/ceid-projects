function [ st ] = modulator(sm, M, T_symbol, T_sample, T_c, E_s)

    N = length(sm);
    samples = T_symbol/T_sample;
    st = zeros(N, samples);
    
    % Calculate Es^1/2 * gT(t)
    Esgt = sqrt(E_s) * sqrt(2 * E_s/T_symbol);
    
    for i = 1:N
        a = sm(i)/M;
        % For each sm(i) caclulate its samples
        for t = 1:samples
            st(i, t) = Esgt * cos(2*pi*(a - t/T_c)); 
        end
    end
end

