% Load input signal x
load('source.mat');
N = length(x);

% Configuration constants
minVal = -3;
maxVal =  3;

% Ploting colors
colors = ['-+b'; '-om'; '-*k'; '-xr'; '-sc'];

% Erwthma 2
r=(1:N);
for p = 4:12:16
    figure
    plot(r, x(r), colors(1,:));
    for i = 1:3
        [~, ~, y] = dpcm_encoder(x, p, i, minVal, maxVal);
        hold on
        plot(r, y(r), colors(1+i,:));
    end
    title(['Error comparison p=', num2str(p)]);
    xlabel('Samples'); ylabel('Prediction y');
    legend('x signal', 'y N=1', 'y N=2', 'y N=3');
end

% Erwthma 3
r=(1:3);
m = zeros(3, 1);
figure
for p = 4:8
    for i = 1:3
        [a_q, ~, y] = dpcm_encoder(x, p, i, minVal, maxVal);
        m(i) = mean(y.^2);
    end
    hold on
    plot(r, m(r), colors(p-3,:));
    % Latex array
    % disp([num2str(p), num2str(a_q',' & %.4f'),'\\']) 
    disp(['p: ',num2str(p),', aq: ', num2str(a_q')])
end
title('Mean squared');
xlabel('N'); ylabel('E(y^2)');
legend('m p=4', 'm p=5', 'm p=6', 'm p=7', 'm p=8');


% Erwthma 4
r = (1:15);
for p = 4:4:8
    figure
    plot(r, x(r), colors(1,:));
    for i = 1:3
        [a_q, yh, ~] = dpcm_encoder(x, p, i, minVal, maxVal);
        yht = dpcm_decoder(a_q, yh);
        hold on
        plot(r, yht(r), colors(1+i,:));
    end
    title(['Signal rebuild p=', num2str(p)]);
    xlabel('Samples'); ylabel('yht');
    legend('x signal', 'yht N=1', 'yht N=2', 'yht N=3');
end