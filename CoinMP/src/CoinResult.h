
/* CoinResult.h */


#ifndef _COINRESULT_H_
#define _COINRESULT_H_


typedef struct {
				int SolutionStatus;
				char SolutionText[200];

				double ObjectValue;
				double MipBestBound;
				int IterCount;
				int MipNodeCount;

				double* ColActivity;
				double* ReducedCost;
				double* RowActivity;
				double* SlackValues;
				double* ShadowPrice;

				double* ObjLoRange;
				double* ObjUpRange;
				double* RhsLoRange;
				double* RhsUpRange;

				int* ColStatus;
				int* RowStatus;
				} RESULTINFO, *PRESULT;

#ifdef __cplusplus
extern "C" {
#endif


PRESULT coinCreateResultObject(void);

void coinClearResultObject(PRESULT pResult);

#ifdef __cplusplus
}
#endif


#endif  /* _COINRESULT_H_ */
