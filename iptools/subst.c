
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "loop_lib.h"
#include "subst.h"
#include "tokparser.h"


TOKENDEF subst_rule_tokens[] =
{
  { SUBST_SRC_PORT,		"src-port" },
  { SUBST_DST_PORT,		"dst-port" },
  { SUBST_SRC_PORT_RANGE,	"src-port-range" },
  { SUBST_DST_PORT_RANGE,	"dst-port-range" }, 
  { SUBST_SRC_IP,		"src-ip" },
  { SUBST_DST_IP,		"dst-ip" },
  { -1, 0 }
};



void error_msg(const char *fmt, ... );

int compile_substitution_rule( SUBST_RULE *rule, const char *srule )
{
   char *copy, *tok = 0;
   int cmd_type;

   if (rule) {
     copy = strdup( srule );

     tok = strtok( copy, ", \t" );
   } else {
     tok = 0;
   }

   if (!tok) {
      error_msg( "Illegal value of -f option. Wrong value is %s", tok ? tok : "Missing value" );
      return -1;
   }

   cmd_type = parse_token(tok, subst_rule_tokens ); 
   if (cmd_type == -1) {
      error_msg( "Illegal command type value. Posible values should be: %s\n", 
			token_possible_values( subst_rule_tokens ) );
      return -1; 
   }
   rule->type = cmd_type;

   switch( cmd_type ) {
    case SUBST_SRC_PORT:
    case SUBST_DST_PORT:
      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Missing source port value in rule string %s", rule);
      }
      rule->v.port_range.from_port_start = htons( (unsigned short) atoi( tok ) );

      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Missing destination port value in rule string %s", rule);
      }
      rule->v.port_range.to_port_start = htons( (unsigned short) atoi( tok ) );
      break;

    case SUBST_SRC_PORT_RANGE:
    case SUBST_DST_PORT_RANGE:
      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Mising minimum source port value in capture port range. rule string %s", rule);
      }
      rule->v.port_range.from_port_start = (unsigned short) atoi( tok );

      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Mising maximum source port value in capture port range. rule string %s", rule);
      }
      rule->v.port_range.from_port_end = (unsigned short) atoi( tok );

      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Mising destinaton port value in rule string %s", rule);
      }
      rule->v.port_range.to_port_start = (unsigned short) atoi( tok );
      break;

    case SUBST_SRC_IP:	
    case SUBST_DST_IP:
      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Mising source ip value in rule string %s", rule);
      }
      if (inet_pton( AF_INET, tok, &rule->v.addr_rule.from_ip) <= 0 ) {
	error_msg( "Illegal value (%s) of source ip address in rule %s", tok, srule  );
      }

      tok = strtok( 0, ", \t" );
      if (!tok) {
	error_msg( "Mising destination ip value in rule string %s", rule);
      }
      if (inet_pton( AF_INET, tok, &rule->v.addr_rule.to_ip) <= 0) {
	error_msg( "Illegal value (%s) of destination ip address in rule %s", tok, srule  );
      }
      break;
   }
   
   return 0; 
}


int apply_substituion_rule( SUBST_RULE *rule, struct ip *ip, struct udphdr *udp, int modify_flag )
{
   unsigned short port;

   switch( rule->type ) {
    case SUBST_SRC_PORT:
	if (udp != 0 && UDP_SPORT( udp ) == rule->v.port_range.from_port_start) {
	  UDP_SPORT( udp ) = rule->v.port_range.to_port_start;
          modify_flag |= MODIFY_FLAG_UDP_MODIFIED;
        }
	break;

    case SUBST_DST_PORT:
	if (udp != 0 && UDP_DPORT( udp ) == rule->v.port_range.from_port_start) {
	  UDP_DPORT( udp ) = rule->v.port_range.to_port_start;
          modify_flag |= MODIFY_FLAG_UDP_MODIFIED;
        }
	break;

    case SUBST_SRC_PORT_RANGE:
	if (udp != 0 ) {
          port = ntohs( UDP_SPORT( udp ) );
   
	  if (port >= rule->v.port_range.from_port_start && 
	      port < rule->v.port_range.from_port_end) {
	    UDP_SPORT( udp ) = htons( (port - rule->v.port_range.from_port_start) + rule->v.port_range.to_port_start );
            modify_flag |= MODIFY_FLAG_UDP_MODIFIED;
          }
	}
	break;

    case SUBST_DST_PORT_RANGE: 
	if (udp != 0 ) {
          port = ntohs( UDP_DPORT( udp ) );
   
	  if (port >= rule->v.port_range.from_port_start && 
	      port < rule->v.port_range.from_port_end) {
	    UDP_DPORT( udp ) = htons( (port - rule->v.port_range.from_port_start) + rule->v.port_range.to_port_start );
            modify_flag |= MODIFY_FLAG_UDP_MODIFIED;
          }
	}
	break;

    case SUBST_SRC_IP:
	if (ip->ip_src.s_addr == rule->v.addr_rule.from_ip.s_addr) {
	  ip->ip_src.s_addr = rule->v.addr_rule.to_ip.s_addr;
          modify_flag |= MODIFY_FLAG_IP_MODIFIED;
        }
	break;

    case SUBST_DST_IP:
	if (ip->ip_dst.s_addr == rule->v.addr_rule.from_ip.s_addr) {
	  ip->ip_dst.s_addr = rule->v.addr_rule.to_ip.s_addr;
          modify_flag |= MODIFY_FLAG_IP_MODIFIED;
        }
	break;
    }
    return modify_flag;
}


#if 0
void revert_substituion_rule( SUBST_RULE *rule, struct ip *ip, struct udphdr *udp )
{
   switch( rule->type ) {
    case SUBST_SRC_PORT:
	break;

    case SUBST_DST_PORT:
	break;

    case SUBST_SRC_PORT_RANGE:
	break;

    case SUBST_DST_PORT_RANGE: 
	break;

    case SUBST_SRC_IP:
	break;

    case SUBST_DST_IP:
	break;
   
    }
}
#endif

