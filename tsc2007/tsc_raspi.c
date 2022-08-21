/*
 * Raspberry Pi TSC2007 Platform Data
 *
 * Copyright (C) 2012 Omar Sandoval
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include "tsc2007.h"
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

static struct i2c_client *tsc2007_client;

static int tsc2007_penirq_pin = 4;
module_param_named(penirq_pin, tsc2007_penirq_pin, int, 0);
MODULE_PARM_DESC(penirq_pin,
    "PENIRQ GPIO pin (17=default)");

static int tsc2007_get_pendown_state(void)
{
	return !gpio_get_value(tsc2007_penirq_pin);
}

static void tsc2007_clear_penirq(void)
{
	gpio_set_value(tsc2007_penirq_pin, 1);
}

static int tsc2007_init_platform_hw(void)
{
	return gpio_request_one(tsc2007_penirq_pin,
				GPIOF_IN, "PENIRQ");
}

static void tsc2007_exit_platform_hw(void)
{
	gpio_free(tsc2007_penirq_pin);
}

static struct tsc2007_platform_data tsc2007_pdata = {
	.model		= 2007,

	.x_plate_ohms	= 300,
	.max_rt		= 1 << 12,

	.poll_delay	= 5,
	.poll_period	= 5,

	.fuzzx		= 64,
	.fuzzy		= 64,
	.fuzzz		= 64,

	.get_pendown_state	= tsc2007_get_pendown_state,
	.clear_penirq		= tsc2007_clear_penirq,
	.init_platform_hw	= tsc2007_init_platform_hw,
	.exit_platform_hw	= tsc2007_exit_platform_hw
};

static struct i2c_board_info raspi_board_info = {
	I2C_BOARD_INFO("tsc2007", 0x48),
	.platform_data = &tsc2007_pdata
};

static int __init tsc_raspi_init(void)
{
	int err;
	struct i2c_adapter *adapter;

	err = gpio_to_irq(tsc2007_penirq_pin);
	if (err < 0)
		goto error;
	raspi_board_info.irq = err;

	err = irq_set_irq_type(raspi_board_info.irq, IRQ_TYPE_EDGE_FALLING);
	if (err < 0)
		goto error;

	adapter = i2c_get_adapter(1);
	if (adapter == NULL) {
	    adapter = i2c_get_adapter(0);
	    if (adapter == NULL) {
	        err = -ENXIO;
		    goto error;
	    }
	}

	tsc2007_client = i2c_new_client_device(adapter, &raspi_board_info);
	err = tsc2007_client ? 0 : -ENXIO;

error:
	return err;
}

static void __exit tsc_raspi_exit(void)
{
	i2c_unregister_device(tsc2007_client);
}

module_init(tsc_raspi_init);
module_exit(tsc_raspi_exit);

MODULE_AUTHOR("Omar Sandoval <osandov@cs.washington.edu>");
MODULE_DESCRIPTION("Raspberry Pi TSC2007 Platform Data");
MODULE_LICENSE("GPL");
