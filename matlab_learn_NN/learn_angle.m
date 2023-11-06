readings_sens2 = importdata("sensors_line_crossing2.csv");
readings_sens1 = importdata("sensors_readings.csv");

angle_x = [readings_sens1(:,1:10); readings_sens2(:,1:10)];

angle_t = [readings_sens1(:,11); readings_sens2(:,11)/100];
histogram(angle_t)