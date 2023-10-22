clear all
close all

model = tf([1480], [0.074 1 0]);
[y, t_out] = step(model, 0.5);
A_read = readmatrix("mess_5000_2000hz_2.csv");
A = (A_read - 200) * 32/180*pi;
left_enc = A(:,1);
right_enc = A(:,2);
time_enc = linspace(0, 0.5, 1000)';
hold on
plot(time_enc, right_enc, 'r-')
plot(time_enc, left_enc, 'b-')
plot(t_out, y, 'y')