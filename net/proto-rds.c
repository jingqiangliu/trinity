#include <stdlib.h>
#include "net.h"
#include "config.h"

#ifdef USE_RDS
#include <linux/rds.h>
#endif

#include "compat.h"
#include "random.h"
#include "utils.h"	// RAND_ARRAY

static void rds_rand_socket(struct socket_triplet *st)
{
	st->protocol = 0;
	st->type = SOCK_SEQPACKET;
}

static void rds_gen_sockaddr(struct sockaddr **addr, socklen_t *addrlen)
{
	struct sockaddr_in *rds;

	rds = zmalloc(sizeof(struct sockaddr_in));

	rds->sin_family = AF_INET;
	rds->sin_addr.s_addr = random_ipv4_address();
	rds->sin_port = rnd() % 65535;

	*addr = (struct sockaddr *) rds;
	*addrlen = sizeof(struct sockaddr_in);
}

#ifdef USE_RDS
static const unsigned int rds_opts[] = {
	RDS_CANCEL_SENT_TO, RDS_GET_MR, RDS_FREE_MR,
	4, /* deprecated RDS_BARRIER 4 */
	RDS_RECVERR, RDS_CONG_MONITOR, RDS_GET_MR_FOR_DEST,
};

#define SOL_RDS 276

static void rds_setsockopt(struct sockopt *so, __unused__ struct socket_triplet *triplet)
{
	so->level = SOL_RDS;
	so->optname = RAND_ARRAY(rds_opts);
}

const struct netproto proto_rds = {
	.name = "rds",
	.socket = rds_rand_socket,
	.setsockopt = rds_setsockopt,
	.gen_sockaddr = rds_gen_sockaddr,
};
#else
/* stub if we are built on something without RDS headers */
void rds_setsockopt(struct sockopt *so)
{
}
#endif	/* USE_RDS */
