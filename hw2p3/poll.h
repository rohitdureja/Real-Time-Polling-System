/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _POLL_H_RPCGEN
#define _POLL_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct Vote {
	char *name;
	int voterid;
};
typedef struct Vote Vote;

struct Candidates {
	char *name;
	struct Candidates *next;
};
typedef struct Candidates Candidates;

#define POLL_PROG 0x23451111
#define POLL_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define zeroize 1
extern  char ** zeroize_1(void *, CLIENT *);
extern  char ** zeroize_1_svc(void *, struct svc_req *);
#define addvoter 2
extern  char ** addvoter_1(int *, CLIENT *);
extern  char ** addvoter_1_svc(int *, struct svc_req *);
#define votefor 3
extern  char ** votefor_1(Vote *, CLIENT *);
extern  char ** votefor_1_svc(Vote *, struct svc_req *);
#define listcandidates 4
extern  Candidates * listcandidates_1(void *, CLIENT *);
extern  Candidates * listcandidates_1_svc(void *, struct svc_req *);
#define votecount 5
extern  int * votecount_1(char **, CLIENT *);
extern  int * votecount_1_svc(char **, struct svc_req *);
extern int poll_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define zeroize 1
extern  char ** zeroize_1();
extern  char ** zeroize_1_svc();
#define addvoter 2
extern  char ** addvoter_1();
extern  char ** addvoter_1_svc();
#define votefor 3
extern  char ** votefor_1();
extern  char ** votefor_1_svc();
#define listcandidates 4
extern  Candidates * listcandidates_1();
extern  Candidates * listcandidates_1_svc();
#define votecount 5
extern  int * votecount_1();
extern  int * votecount_1_svc();
extern int poll_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Vote (XDR *, Vote*);
extern  bool_t xdr_Candidates (XDR *, Candidates*);

#else /* K&R C */
extern bool_t xdr_Vote ();
extern bool_t xdr_Candidates ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_POLL_H_RPCGEN */
