function [ smh ] = detector(r, M)

    [symbols, ~] = size(r);
    smh = zeros(symbols, 1);
    dist = zeros(M, 1);
    sm = zeros(M, 2);
    
    % Calculate each symbol's position
    for m = 1:M
        sm(m,:) = [cos(2*pi*m/M), sin(2*pi*m/M)];
    end
    
    for i = 1:symbols
        % For each symbol in sm calculate its euclidian
        % distance from the r(i) symbol
        for m = 1:M
            dist(m) = norm(r(i,:) - sm(m,:));
        end
        % Find the closest approximation
        [~, smh(i)] = min(dist);
    end
    smh = mod(smh, M);
end

