#ifndef __FRAGMENTS_X_Y_Z__
#define __FRAGMENTS_X_Y_Z__

int init_fragments( HASH *fragments );


int process_fragments( PACKET_DATA *data, HASH *fragments, IP_FRAGMENT_ACTION fragment_action );

#endif


