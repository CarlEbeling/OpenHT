#include "Ht.h"
#include "PersModC.h"

void CPersModC::PersModC()
{
	if (PR_htValid) {
		switch (PR_htInst) {
		case MODC:
		{
			if (SendReturnBusy_modC()) {
				HtRetry();
				break;
			}

			// Return to host interface
			SendReturn_modC();
		}
		break;
		default:
			assert(0);
		}
	}

	if (!GR_htReset && !RecvMsgBusy_BtoC()) {
		ht_uint4 msg = RecvMsg_BtoC();
		HtAssert((msg >> 2) == i_replId.read()/(C_REPL/B_REPL)+1u, msg);
		HtAssert((msg & 0x3) == i_replId.read()%(C_REPL/B_REPL)+1u, msg);

		bool bMsgSendBusy = SendMsgBusy_CtoD();
		HtAssert(!bMsgSendBusy, 0);
		if (!bMsgSendBusy)
			SendMsg_CtoD(msg);
	}
}
