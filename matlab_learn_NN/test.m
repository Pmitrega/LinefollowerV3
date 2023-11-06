%y = myNeuralNetworkFunction([1200,1200,64]);
readings = importdata("data_PWM_2.csv");
readings_sens = importdata("sensors_readings.csv");
histogram(readings(:,5)/100, linspace(-40,40, 40));
n = dec2bin(16,10);
x = [readings(:,5)/100, readings(:, 1), readings(:,2)]';
t = [readings(:,3), readings(:,4)]';