readings_sens2 = importdata("sensors_line_crossing2.csv");
readings_sens1 = importdata("sensors_readings.csv");

angle_x = [readings_sens1(:,1:10)/1000; readings_sens2(:,1:10)/1000];
close all
angle_t = [readings_sens1(:,11); readings_sens2(:,11)/100];
figure
plot([angle_t(1:200, :); angle_t(295:426, :)]*.7, [angle_x(1:200,:); angle_x(295:426, :)], ".")
ylabel("Znormalizowane pomiary z czujników")
xlabel('Odchylenie od linii [$^{\circ}$]','Interpreter','latex')
figure
histogram(angle_t)