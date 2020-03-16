% Load input signal x
load('source.mat');
N = length(x);

% Configuration constants
minVal = -3;
maxVal =  3;

% Erwthma 2
r=(1:N);
for p = 4:12:16
    y = zeros(3, N);
    for i = 1:3
        [a_q, yh, y(i,:)] = dpcm_encoder(x, p, i, minVal, maxVal);
    end
    figure
    plot(r, x(r), 'b', r, y(1,:),'m', r, y(2,:),'k', r, y(3,:),'r');
    xlabel('Samples'); 
    ylabel('Prediction y');
    title(['Error comparison p=', num2str(p)]);
    legend('x signal', 'y N=1', 'y N=2', 'y N=3');
end

% Erwthma 3
r=(1:3);
m = zeros(5, 3);
figure
for p = 4:8
    for i = 1:3
        [a_q, yh, y] = dpcm_encoder(x, p, i, minVal, maxVal);
        m(p-3, i) = mean(y.^2);
    end
    disp(a_q); 
end
title('Mean squared');
plot(r, m(1,:), r, m(2,:), r, m(3,:), r, m(4,:), r, m(5,:));
xlabel('N'); ylabel('E(y^2)');
legend('m p=4', 'm p=5', 'm p=6', 'm p=7', 'm p=8');


% Erwthma 4
r = (1:15);
for p = 4:4:8
    figure
    yht = zeros(3, N);
    for i = 1:3
        [a_q, yh, y] = dpcm_encoder(x, p, i, minVal, maxVal);
        yht(i,:) = dpcm_decoder(a_q, yh);
    end
    plot(r, x(r), r, yht(1,r), r, yht(2,r), r, yht(3,r));
    title(['Signal rebuild p=', num2str(p)]);
    xlabel('Samples'); ylabel('yht');
    legend('x signal', 'yht N=1', 'yht N=2', 'yht N=3');
end