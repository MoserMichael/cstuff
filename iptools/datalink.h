#ifndef __DATALINK_H_Y_Z__
#define __DATALINK_H_Y_Z__

#include <stdint.h>
#include <stdlib.h>

int datalink_is_type_supported(int datalink_type);
int datalink_skip_layer_data(  int idx,  const uint8_t **frame, size_t *size );
void datalink_print_supported_types();

#endif

