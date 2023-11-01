%y = myNeuralNetworkFunction([1200,1200,64]);
load readings4.csv
sensors = readings4(:,5);
histogram(sensors, 1023)
dec2bin(16,10)
readings3 = readings4;
x = [readings3(:,1)/2400, readings3(:,2)/2400 readings3(:,5)]';
t = [readings3(:,3)/2400, readings3(:,4)/2400]';