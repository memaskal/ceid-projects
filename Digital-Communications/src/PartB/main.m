% Simulation constants
T_symbol = 40; T_c = 4;
T_sample = 1; E_s = 1;
SNR = 0:2:16;

% Input generator
Lb = randi([1e4 1e5], 1, 1);
x = randsrc(Lb, 1, [0 1]);

% Plotting colors
colors = ['-+b'; '-dk'; '-sm'; '-xr'];
msr = zeros(2, 9);

figure
for i = 0:1
for M = 4:4:8
    for snr = SNR
        % M-PSK transmitter 
        sm = mapper(x, M, i); 
        st = modulator(sm, M, T_symbol, T_sample, T_c, E_s); 
        % AWGN Channel
        rt = noise_generator(st, M, E_s, snr);
        % M-PSK Receiver
        r = demodulator(rt, T_symbol, T_c, E_s);
        smh = detector(r, M);
        xh = demapper(smh, M, i);
        % BER calculation
        [~, msr(M/4, snr/2+1)] = biterr(x, xh(1:Lb));
    end
end

% Plot the measurements
if (i == 1) hold on; end
semilogy(SNR, msr(1,:), colors(i*2+1,:),...
         SNR, msr(2,:), colors(i*2+2,:));
end

ylabel('BER'); xlabel('SNR(db)');
legend('M=4 normal enc', 'M=8 normal enc',...
       'M=4 gray enc', 'M=8 gray enc');

