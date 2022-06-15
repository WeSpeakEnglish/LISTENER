Fsamp = 26000;
MaxSpecFreq = 1000;
scf(1);
clf(1);


exec('back_right.sci',-1);
SizeArray = size(a1);
SizeArray = SizeArray(1);
signal = a1 - mean(a1);
spec_signal = abs(fft(signal));
MaxPlotPoint =round(MaxSpecFreq*SizeArray/Fsamp) 
 for i = 1:1:MaxPlotPoint
    x(i) = (Fsamp/SizeArray)*(i-1);
 end
 subplot(4,1,1);
 plot(x(1:MaxPlotPoint),2*spec_signal(1:MaxPlotPoint)/SizeArray,'r');
 xtitle('The back-right wheel');
 xgrid(100);

exec('back_left.sci',-1);
SizeArray = size(a1);
SizeArray = SizeArray(1);
signal = a1 - mean(a1);
spec_signal = abs(fft(signal));
MaxPlotPoint =round(MaxSpecFreq*SizeArray/Fsamp) 
 for i = 1:1:MaxPlotPoint
    x(i) = (Fsamp/SizeArray)*(i-1);
 end
 subplot(4,1,2);
 plot(x(1:MaxPlotPoint),2*spec_signal(1:MaxPlotPoint)/SizeArray,'b');
 xtitle('The back-left wheel');
 xgrid(100);
 
 exec('front_right.sci',-1);
SizeArray = size(a1);
SizeArray = SizeArray(1);
signal = a1 - mean(a1);
spec_signal = abs(fft(signal));
MaxPlotPoint =round(MaxSpecFreq*SizeArray/Fsamp) 
 for i = 1:1:MaxPlotPoint
    x(i) = (Fsamp/SizeArray)*(i-1);
 end
 subplot(4,1,3);
 plot(x(1:MaxPlotPoint),2*spec_signal(1:MaxPlotPoint)/SizeArray,'r');
 xtitle('The front-right wheel');
 xgrid(100);
 
  exec('front_left.sci',-1);
SizeArray = size(a1);
SizeArray = SizeArray(1);
signal = a1 - mean(a1);
spec_signal = abs(fft(signal));
MaxPlotPoint =round(MaxSpecFreq*SizeArray/Fsamp) 
 for i = 1:1:MaxPlotPoint
    x(i) = (Fsamp/SizeArray)*(i-1);
 end
 subplot(4,1,4);
 plot(x(1:MaxPlotPoint),2*spec_signal(1:MaxPlotPoint)/SizeArray,'b');
 xtitle('The front-left wheel');
 xgrid(100);
 
 