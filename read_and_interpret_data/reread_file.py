
f = open("data_PWM_3.txt", "r")

"""
    struct LogData
{
    int16_t velocity_l;
    int16_t velocity_r;
    int16_t velocity_l_ctrl;
    int16_t velocity_r_ctrl;
    uint16_t sensors;
    uint16_t end;
};
"""
sens = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
k = 0
is_read_all = False

csv_file = open("data_PWM_3.csv", "w")
# print(type(csv_file))
frame = [0, 0, 0, 0, 0]


def decToBinary(n):
    binaryNum = [0] * 10
    i = 0
    while (i < 10):
        binaryNum[i] = n % 2
        n = int(n / 2)
        i += 1
    return binaryNum


def save_frame(file, dat_frame):
    file.write(str(dat_frame[0]) + ", ")
    file.write(str(dat_frame[1]) + ", ")
    file.write(str(dat_frame[2]) + ", ")
    file.write(str(dat_frame[3]) + ", ")
    file.write(str(dat_frame[4]) + "\n")


for i in range(1, 50000):
    try:
        for j in range(5):
            read_val = f.read(4)[::-1]
            if read_val is None:
                is_read_all = True
                break
            read_val = read_val[0:2][::-1] + read_val[2:4][::-1]
            if j < 4:
                # print(read_val, end=" ")
                frame[j] = int(read_val, 16)
                if frame[j] > 50000:
                    frame[j] = frame[j] - 65535
                print(str(int(read_val, 16)), end=" ")
            else:
                frame[4] = int(read_val, 16)
                if frame[4] > 64000:
                    frame[4] = frame[4] - 65536
                int_val = int(read_val, 16)
                sens = decToBinary(int_val)
                print(sens, end=" ")
        save_frame(csv_file, frame)
        read_val = f.read(4)[::-1]
        read_val = read_val[0:2][::-1] + read_val[2:4][::-1]
        print("\n" + str(read_val))
        print(k)
    except:
        break

    k += 1

print(k)
f.close()
csv_file.close()
