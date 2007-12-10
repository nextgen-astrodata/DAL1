%% READ TBB

% make sure we get a clear start

clear all

% length of a block of data input into the FFT (?)
fft_length=2048;

% The number of blocks we are going to read
blocks=4;
fs=160;

fas = ((1:1:fft_length/2)*fs)/fft_length;
lsout = dir([ '19*.dat']);
[maxx dummy] = size(lsout);
nfiles = maxx;

for cnt=1:nfiles
    datafiles(cnt,:) = lsout(cnt).name;
end

% set the total number of samples
samples=fft_length*blocks

% weights of the Hanning window
win = hanning(fft_length);

%idx=8;
idx = 2
datafiles(idx,:)
filename_r=[datafiles(idx,:)];
fid = fopen(filename_r, 'r');

data_r = fread(fid, blocks*fft_length, 'int16');

fclose(fid);

data  = reshape(data_r, fft_length, blocks).';

wn = repmat(win,1,blocks);
w_data = data.*wn';

fdata = fftshift(fft((w_data),fft_length,2))/fft_length;
fdata_m = mean(fdata,1)+1e-12;
figure(idx)
subplot(2,1,1)
plot(data(1,:))
xlabel('sample nr')
ylabel('ADC level')
grid on
title(['TBB data from file ' filename_r ]);
subplot(2,1,2)
plot(fas, 20*log10(abs(fdata_m((fft_length/2)+1:end))));
xlabel('Frequency MHz')
ylabel('Power dB_{adc unit}')
grid on
title(['Averaged Spectrum of ' int2str(blocks) ' x ' int2str(fft_length) ' samples'  ]);
