First, let's talk about the gpio_config_t struct fields. <br> <br>
1- **.mode**: to set the mode to input or output or both. in case of button its input only, in case of LED its ouput only.. etc.<br>
2- **.pin_bit_mask**: to specifiy what pins you want the config to apply to. so if you want pin 5, set it to 1ULL << 5 where ULL means unsigned long long. so we have an unsigned long long of value 1, so we have 63 0s then one, 000000.....00001. then we left shift it 5 times, we then have 00000....10000. As evident the fifth bit is set to 1. In order to set more than one pin to 1 we can apply bit OR to the bit mask, say we want 5 and 3, then we apply the operation (1ULL<<3) | (1ULL<<5), look up how OR works to understand why it works.<br>
3- **pull_up_en**: to specifiy whether to enable or disable pull up, its needed when you have your peripheral connected to GND (ground)<br>
4- **.pull_down_en**: same as 3 for pull down, needed when peripheral connected to VCC<br>
5- **.intr_type**:  when to trigger the interrupt, choices are, a falling or raising edge, a low, and a high.<br>
then call the **gpio_config** function.<br><br>

To create an interrupt, there are multiple functions we can use, https://tinyurl.com/38sxbsfy, one of which is **gpio_install_isr_service**, which can take 0 or ESP_INTR_FLAG_IRAM. <br>
to choose the ISR function and the pins on which the interrupt is triggered, we call **gpio_isr_handler_add**<br><br>

ISR functions have to have [IRAM_ATTR](https://tinyurl.com/38sxbsfy) prefixed the function name, which forces the code to live in the IRAM<br>


