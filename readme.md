## Texture Formats
This is a small C++ project in which I implemented a few texture formats.  
I wanted to test them out, evaluate, and compare their precision.

### Implementated Formats
Below I have a list of the formats I tested with.  
I've detailed their error statistics over a range from `0.0` to `1000.0`.  
*Their precision decreases the larger the numbers get.*

|     Format    | Footprint | Error Average | Error Deviation | Error Minimum | Error Maximum |
|:-------------:|:---------:|:-------------:|:---------------:|:-------------:|:-------------:|
|  `r11g11b10`  |  32 bits  |    1.765508   |     0.908546    |    0.016722   |    5.176900   |
|    `rgb9e5`   |  32 bits  |    0.463239   |     0.185760    |    0.025208   |    0.971992   |
| `rgb9e5_sqrt` |  32 bits  |    0.644466   |     0.285649    |    0.028582   |    1.643005   |
