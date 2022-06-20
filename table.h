// table for delays before PWM rise and fall on 16 bits
uint16_t dPWMdelay[32] = {0};

// flag to check if delay is expired or not
bool bPWMphase[32] = {0};

// index for the rise and fall table
uint16_t dPWMidx[32] = {0};

// Brightness coefficient, this works only with GCC
uint8_t dPWMbrightnessCoef[32] = {[0 ... 31] = 255};

// which array for PWM wave
uint8_t dPWMWaveArray[32] = {0};

const uint8_t * pPWMarray[32];

enum BrightnessType {
    RANDOM,
    LINEAR,
    NORMAL
};

static const uint8_t byte_array_dec0[512u] = {1, 3, 5, 7, 
9,  11, 13, 15, 
17, 19, 21, 23, 
25, 27, 29, 31, 
33, 35, 37, 39, 
41, 43, 45, 47, 
49, 51, 53, 55, 
57, 59, 61, 63, 
65, 67, 69, 71, 
73, 75, 77, 79, 
81, 83, 85, 87, 
89, 91, 93, 95, 
97, 99, 101, 103, 
105, 107, 109, 111, 
113, 115, 117, 119, 
121, 123, 125, 127, 
129, 131, 133, 135, 
137, 139, 141, 143, 
145, 147, 149, 151, 
153, 155, 157, 159, 
161, 163, 165, 167, 
169, 171, 173, 175, 
177, 179, 181, 183, 
185, 187, 189, 191, 
193, 195, 197, 199, 
201, 203, 205, 207, 
209, 211, 213, 215, 
217, 219, 221, 223, 
225, 227, 229, 231, 
233, 235, 237, 239, 
241, 243, 245, 247, 
249, 251, 253, 255,
255, 253, 251, 249, 
247, 245, 243, 241, 
239, 237, 235, 233, 
231, 229, 227, 225, 
223, 221, 219, 217, 
215, 213, 211, 209, 
207, 205, 203, 201, 
199, 197, 195, 193, 
191, 189, 187, 185, 
183, 181, 179, 177, 
175, 173, 171, 169, 
167, 165, 163, 161, 
159, 157, 155, 153, 
151, 149, 147, 145, 
143, 141, 139, 137, 
135, 133, 131, 129, 
127, 125, 123, 121, 
119, 117, 115, 113, 
111, 109, 107, 105, 
103, 101, 99, 97, 
95, 93, 91, 89, 
87, 85, 83, 81, 
79, 77, 75, 73, 
71, 69, 67, 65, 
63, 61, 59, 57, 
55, 53, 51, 49, 
47, 45, 43, 41, 
39, 37, 35, 33, 
31, 29, 27, 25, 
23, 21, 19, 17, 
15, 13, 11, 9, 
7, 5, 3, 1, 0};

static const uint8_t byte_array_dec1[768u] = {1, 2, 3, 4, 5, 6, 7, 
8, 9, 10, 11, 12, 13, 14, 15, 
16, 17, 18, 19, 20, 21, 22, 23, 
24, 25, 26, 27, 28, 29, 30, 31, 
32, 33, 34, 35, 36, 37, 38, 39, 
40, 41, 42, 43, 44, 45, 46, 47, 
48, 49, 50, 51, 52, 53, 54, 55, 
56, 57, 58, 59, 60, 61, 62, 63, 
64, 65, 66, 67, 68, 69, 70, 71, 
72, 73, 74, 75, 76, 77, 78, 79, 
80, 81, 82, 83, 84, 85, 86, 87, 
88, 89, 90, 91, 92, 93, 94, 95, 
96, 97, 98, 99, 100, 101, 102, 103, 
104, 105, 106, 107, 108, 109, 110, 111, 
112, 113, 114, 115, 116, 117, 118, 119, 
120, 121, 122, 123, 124, 125, 126, 127, 
128, 129, 130, 131, 132, 133, 134, 135, 
136, 137, 138, 139, 140, 141, 142, 143, 
144, 145, 146, 147, 148, 149, 150, 151, 
152, 153, 154, 155, 156, 157, 158, 159, 
160, 161, 162, 163, 164, 165, 166, 167, 
168, 169, 170, 171, 172, 173, 174, 175, 
176, 177, 178, 179, 180, 181, 182, 183, 
184, 185, 186, 187, 188, 189, 190, 191, 
192, 193, 194, 195, 196, 197, 198, 199, 
200, 201, 202, 203, 204, 205, 206, 207, 
208, 209, 210, 211, 212, 213, 214, 215, 
216, 217, 218, 219, 220, 221, 222, 223, 
224, 225, 226, 227, 228, 229, 230, 231, 
232, 233, 234, 235, 236, 237, 238, 239, 
240, 241, 242, 243, 244, 245, 246, 247, 
248, 249, 250, 251, 252, 253, 254, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 254, 253, 252, 251, 250, 249, 248, 
247, 246, 245, 244, 243, 242, 241, 240, 
239, 238, 237, 236, 235, 234, 233, 232, 
231, 230, 229, 228, 227, 226, 225, 224, 
223, 222, 221, 220, 219, 218, 217, 216, 
215, 214, 213, 212, 211, 210, 209, 208, 
207, 206, 205, 204, 203, 202, 201, 200, 
199, 198, 197, 196, 195, 194, 193, 192, 
191, 190, 189, 188, 187, 186, 185, 184, 
183, 182, 181, 180, 179, 178, 177, 176, 
175, 174, 173, 172, 171, 170, 169, 168, 
167, 166, 165, 164, 163, 162, 161, 160, 
159, 158, 157, 156, 155, 154, 153, 152, 
151, 150, 149, 148, 147, 146, 145, 144, 
143, 142, 141, 140, 139, 138, 137, 136, 
135, 134, 133, 132, 131, 130, 129, 128, 
127, 126, 125, 124, 123, 122, 121, 120, 
119, 118, 117, 116, 115, 114, 113, 112, 
111, 110, 109, 108, 107, 106, 105, 104, 
103, 102, 101, 100, 99, 98, 97, 96, 
95, 94, 93, 92, 91, 90, 89, 88, 
87, 86, 85, 84, 83, 82, 81, 80, 
79, 78, 77, 76, 75, 74, 73, 72, 
71, 70, 69, 68, 67, 66, 65, 64, 
63, 62, 61, 60, 59, 58, 57, 56, 
55, 54, 53, 52, 51, 50, 49, 48, 
47, 46, 45, 44, 43, 42, 41, 40, 
39, 38, 37, 36, 35, 34, 33, 32, 
31, 30, 29, 28, 27, 26, 25, 24, 
23, 22, 21, 20, 19, 18, 17, 16, 
15, 14, 13, 12, 11, 10, 9, 8, 
7, 6, 5, 4, 3, 2, 1, 0};

static const uint8_t byte_array_dec2[1024u] = {1, 2, 3, 4, 5, 6, 7, 
8, 9, 10, 11, 12, 13, 14, 15, 
16, 17, 18, 19, 20, 21, 22, 23, 
24, 25, 26, 27, 28, 29, 30, 31, 
32, 33, 34, 35, 36, 37, 38, 39, 
40, 41, 42, 43, 44, 45, 46, 47, 
48, 49, 50, 51, 52, 53, 54, 55, 
56, 57, 58, 59, 60, 61, 62, 63, 
64, 65, 66, 67, 68, 69, 70, 71, 
72, 73, 74, 75, 76, 77, 78, 79, 
80, 81, 82, 83, 84, 85, 86, 87, 
88, 89, 90, 91, 92, 93, 94, 95, 
96, 97, 98, 99, 100, 101, 102, 103, 
104, 105, 106, 107, 108, 109, 110, 111, 
112, 113, 114, 115, 116, 117, 118, 119, 
120, 121, 122, 123, 124, 125, 126, 127, 
128, 129, 130, 131, 132, 133, 134, 135, 
136, 137, 138, 139, 140, 141, 142, 143, 
144, 145, 146, 147, 148, 149, 150, 151, 
152, 153, 154, 155, 156, 157, 158, 159, 
160, 161, 162, 163, 164, 165, 166, 167, 
168, 169, 170, 171, 172, 173, 174, 175, 
176, 177, 178, 179, 180, 181, 182, 183, 
184, 185, 186, 187, 188, 189, 190, 191, 
192, 193, 194, 195, 196, 197, 198, 199, 
200, 201, 202, 203, 204, 205, 206, 207, 
208, 209, 210, 211, 212, 213, 214, 215, 
216, 217, 218, 219, 220, 221, 222, 223, 
224, 225, 226, 227, 228, 229, 230, 231, 
232, 233, 234, 235, 236, 237, 238, 239, 
240, 241, 242, 243, 244, 245, 246, 247, 
248, 249, 250, 251, 252, 253, 254, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 254, 253, 252, 251, 250, 249, 248, 
247, 246, 245, 244, 243, 242, 241, 240, 
239, 238, 237, 236, 235, 234, 233, 232, 
231, 230, 229, 228, 227, 226, 225, 224, 
223, 222, 221, 220, 219, 218, 217, 216, 
215, 214, 213, 212, 211, 210, 209, 208, 
207, 206, 205, 204, 203, 202, 201, 200, 
199, 198, 197, 196, 195, 194, 193, 192, 
191, 190, 189, 188, 187, 186, 185, 184, 
183, 182, 181, 180, 179, 178, 177, 176, 
175, 174, 173, 172, 171, 170, 169, 168, 
167, 166, 165, 164, 163, 162, 161, 160, 
159, 158, 157, 156, 155, 154, 153, 152, 
151, 150, 149, 148, 147, 146, 145, 144, 
143, 142, 141, 140, 139, 138, 137, 136, 
135, 134, 133, 132, 131, 130, 129, 128, 
127, 126, 125, 124, 123, 122, 121, 120, 
119, 118, 117, 116, 115, 114, 113, 112, 
111, 110, 109, 108, 107, 106, 105, 104, 
103, 102, 101, 100, 99, 98, 97, 96, 
95, 94, 93, 92, 91, 90, 89, 88, 
87, 86, 85, 84, 83, 82, 81, 80, 
79, 78, 77, 76, 75, 74, 73, 72, 
71, 70, 69, 68, 67, 66, 65, 64, 
63, 62, 61, 60, 59, 58, 57, 56, 
55, 54, 53, 52, 51, 50, 49, 48, 
47, 46, 45, 44, 43, 42, 41, 40, 
39, 38, 37, 36, 35, 34, 33, 32, 
31, 30, 29, 28, 27, 26, 25, 24, 
23, 22, 21, 20, 19, 18, 17, 16, 
15, 14, 13, 12, 11, 10, 9, 8, 
7, 6, 5, 4, 3, 2, 1, 0};

static const uint8_t byte_array_dec3[1280u] = {1, 2, 3, 4, 5, 6, 7, 
8, 9, 10, 11, 12, 13, 14, 15, 
16, 17, 18, 19, 20, 21, 22, 23, 
24, 25, 26, 27, 28, 29, 30, 31, 
32, 33, 34, 35, 36, 37, 38, 39, 
40, 41, 42, 43, 44, 45, 46, 47, 
48, 49, 50, 51, 52, 53, 54, 55, 
56, 57, 58, 59, 60, 61, 62, 63, 
64, 65, 66, 67, 68, 69, 70, 71, 
72, 73, 74, 75, 76, 77, 78, 79, 
80, 81, 82, 83, 84, 85, 86, 87, 
88, 89, 90, 91, 92, 93, 94, 95, 
96, 97, 98, 99, 100, 101, 102, 103, 
104, 105, 106, 107, 108, 109, 110, 111, 
112, 113, 114, 115, 116, 117, 118, 119, 
120, 121, 122, 123, 124, 125, 126, 127, 
128, 129, 130, 131, 132, 133, 134, 135, 
136, 137, 138, 139, 140, 141, 142, 143, 
144, 145, 146, 147, 148, 149, 150, 151, 
152, 153, 154, 155, 156, 157, 158, 159, 
160, 161, 162, 163, 164, 165, 166, 167, 
168, 169, 170, 171, 172, 173, 174, 175, 
176, 177, 178, 179, 180, 181, 182, 183, 
184, 185, 186, 187, 188, 189, 190, 191, 
192, 193, 194, 195, 196, 197, 198, 199, 
200, 201, 202, 203, 204, 205, 206, 207, 
208, 209, 210, 211, 212, 213, 214, 215, 
216, 217, 218, 219, 220, 221, 222, 223, 
224, 225, 226, 227, 228, 229, 230, 231, 
232, 233, 234, 235, 236, 237, 238, 239, 
240, 241, 242, 243, 244, 245, 246, 247, 
248, 249, 250, 251, 252, 253, 254, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255, 
255, 255, 255, 255, 255, 255, 255, 255,
255, 254, 253, 252, 251, 250, 249, 248, 
247, 246, 245, 244, 243, 242, 241, 240, 
239, 238, 237, 236, 235, 234, 233, 232, 
231, 230, 229, 228, 227, 226, 225, 224, 
223, 222, 221, 220, 219, 218, 217, 216, 
215, 214, 213, 212, 211, 210, 209, 208, 
207, 206, 205, 204, 203, 202, 201, 200, 
199, 198, 197, 196, 195, 194, 193, 192, 
191, 190, 189, 188, 187, 186, 185, 184, 
183, 182, 181, 180, 179, 178, 177, 176, 
175, 174, 173, 172, 171, 170, 169, 168, 
167, 166, 165, 164, 163, 162, 161, 160, 
159, 158, 157, 156, 155, 154, 153, 152, 
151, 150, 149, 148, 147, 146, 145, 144, 
143, 142, 141, 140, 139, 138, 137, 136, 
135, 134, 133, 132, 131, 130, 129, 128, 
127, 126, 125, 124, 123, 122, 121, 120, 
119, 118, 117, 116, 115, 114, 113, 112, 
111, 110, 109, 108, 107, 106, 105, 104, 
103, 102, 101, 100, 99, 98, 97, 96, 
95, 94, 93, 92, 91, 90, 89, 88, 
87, 86, 85, 84, 83, 82, 81, 80, 
79, 78, 77, 76, 75, 74, 73, 72, 
71, 70, 69, 68, 67, 66, 65, 64, 
63, 62, 61, 60, 59, 58, 57, 56, 
55, 54, 53, 52, 51, 50, 49, 48, 
47, 46, 45, 44, 43, 42, 41, 40, 
39, 38, 37, 36, 35, 34, 33, 32, 
31, 30, 29, 28, 27, 26, 25, 24, 
23, 22, 21, 20, 19, 18, 17, 16, 
15, 14, 13, 12, 11, 10, 9, 8, 
7, 6, 5, 4, 3, 2, 1, 0};