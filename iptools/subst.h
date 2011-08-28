/* Copyright NDS Group - 2011 */
#ifndef __SUBST_H_
#define __SUBST_H_



#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

typedef enum tagSUBST_RULE_TYPE { 
  
  SUBST_SRC_PORT,
  SUBST_DST_PORT,
  SUBST_SRC_PORT_RANGE,
  SUBST_DST_PORT_RANGE, 
  SUBST_SRC_IP,
  SUBST_DST_IP,

} SUBST_RULE_TYPE;



#define MODIFY_FLAG_IP_MODIFIED  0x1
#define MODIFY_FLAG_UDP_MODIFIED 0x2

typedef struct tagSUBST_RULE {
  SUBST_RULE_TYPE type;
  union {
    struct {
	int from_port_start;
	int from_port_end;
        int to_port_start;
    } port_range;

    struct {
	struct in_addr from_ip;
	struct in_addr to_ip;
    } addr_rule;
  } v;
} SUBST_RULE;


#define MAKE_SOURCE_RULE( rule, x, y ) do { \
	(rule)->type = SUBST_SRC_IP; \
	(rule)->v.addr_rule.from_ip.s_addr = (x).s_addr; \
	(rule)->v.addr_rule.to_ip.s_addr = (y).s_addr; \
 } while(0);

int  compile_substitution_rule( SUBST_RULE *rule, const char *srule );
int apply_substituion_rule( SUBST_RULE *rule, struct ip *ip, struct udphdr *udp, int modify_flag );
void revert_substituion_rule( SUBST_RULE *rule, struct ip *ip, struct udphdr *udp );

#endif

