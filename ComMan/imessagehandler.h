
#if !defined(AFX_IMESSAGEHANDLER_H__375D114C_CE19_4DD9_A2C4_BF3F6D5B789B__INCLUDED_)
#define AFX_IMESSAGEHANDLER_H__375D114C_CE19_4DD9_A2C4_BF3F6D5B789B__INCLUDED_

namespace cs
{
	struct imessagehandler
	{
		virtual int HandleMessage( BYTE aType, BYTE* aData, int aDataSize, DWORD aCheckSum) = 0;
		virtual bool IsSynchMessage( BYTE aType){ if(aType > 0) return false; return false;}
	};

#define HM_WRONG_TYPE -1
#define HM_NOT_ENOUGH_DATA -2
#define HM_SYNCH_DATA -3
#define HM_NOT_ENOUGH_DATA_SYNCH -4

#define BEGIN_IN_MESSAGE_MAP() \
	virtual int HandleMessage( BYTE aType, BYTE* aData, int aDataSize, DWORD aCheckSum) \
	{ \
		switch (aType) \
		{
#define IN_MESSAGE2( TYPE, HANDLER) \
			case TYPE: \
			{ \
				if (aDataSize < 4) \
					return HM_NOT_ENOUGH_DATA; \
				int nSize = (*(int*) aData); \
				if (aDataSize - 4 < nSize) \
					return HM_NOT_ENOUGH_DATA; \
				HANDLER( aData + 4, nSize); \
				aCheckSum = 0;\
				return nSize + 4; \
			}
#define IN_MESSAGE2_CHECKSUM( TYPE, HANDLER) \
			case TYPE: \
			{ \
				if (aDataSize < 4) \
					return HM_NOT_ENOUGH_DATA; \
				int nSize = (*(int*) aData); \
				if (aDataSize - 4 < nSize) \
					return HM_NOT_ENOUGH_DATA; \
				HANDLER( aData + 4, nSize, aCheckSum); \
				return nSize + 4; \
			}
#define IN_MESSAGE_SYNCH( TYPE, HANDLER) \
			case TYPE: \
			{ \
				if (aDataSize < 4) \
					return HM_NOT_ENOUGH_DATA_SYNCH; \
				int nSize = (*(int*) aData); \
				if (aDataSize - 4 < nSize) \
					return HM_NOT_ENOUGH_DATA_SYNCH; \
				HANDLER( aData + 4, nSize); \
				aCheckSum = 0;\
				return HM_SYNCH_DATA; \
			}
#define IN_MESSAGE_ONE_OF_RANGE( TYPE, HANDLER) \
			case TYPE: \
			{ \
				if (aDataSize < 4) \
					return HM_NOT_ENOUGH_DATA; \
				int nSize = (*(int*) aData); \
				if (aDataSize - 4 < nSize) \
					return HM_NOT_ENOUGH_DATA; \
				HANDLER( TYPE, aData + 4, nSize, aCheckSum); \
				return nSize + 4; \
			}
#define END_IN_MESSAGE_MAP() \
		} \
		return HM_WRONG_TYPE; \
	}

#define BEGIN_OUT_MESSAGE_MAP() \
	virtual int GetOutMessageSize( BYTE aType) \
	{ \
		switch (aType) \
		{
#define OUT_MESSAGE( TYPE, MES_SIZE) \
			case TYPE: return MES_SIZE;
#define END_OUT_MESSAGE_MAP() \
		} \
		return -1; \
	}

#define BEGIN_SYNCH_MESSAGE_MAP() \
	virtual bool IsSynchMessage( BYTE aType) \
	{ \
		switch (aType) \
		{

#define IS_SYNCH( TYPE) \
			case TYPE: \
				return true;

#define END_SYNCH_MESSAGE_MAP() \
		} \
		return false; \
	}


}
#endif 