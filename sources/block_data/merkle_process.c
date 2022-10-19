#include "merkle_process.h"

int merkle_verify_message(unsigned long long EXPONENT, size_t msg_num, layer_hp *Layers_pointer)
{

    size_t req_msg_first_nodeN = (msg_num >> 1LU); // assign node number
                                                   //    size_t req_msg_first_nodeN = (msg_num );  // assign node number
    int flip = (req_msg_first_nodeN % 2LU) ? 0 : 1; // flip hpoint if % is true num is level 1 num
    printf("TESTING N MSG:%lu flip = %d \n", req_msg_first_nodeN, flip);
    size_t UPLOAD_req_msg_first_nodeN = req_msg_first_nodeN + 1LU;
    printf("level:%lu\n", (Layers_pointer[EXPONENT - 1]).level);
    unsigned char *hash_msg1_f =
        calc_hash(*(*(Layers_pointer[EXPONENT - 1].main_pointer[req_msg_first_nodeN])).messages.smsg_p1);
    unsigned char *Shash2 = calloc(crypto_generichash_BYTES, sizeof(unsigned char));
    unsigned char *hash_msg2_f =
        calc_hash(*(*(Layers_pointer[EXPONENT - 1].main_pointer[req_msg_first_nodeN])).messages.smsg_p2);
    unsigned char *Shash = merge_2hashses(hash_msg1_f, hash_msg2_f); // CALC SHASH
    int ret_gr_lev = 0;
    if (flip)
    {
        ret_gr_lev = memcmp(
            Shash,
            ((hash_point_p)((*(Layers_pointer[EXPONENT - 2].main_pointer[req_msg_first_nodeN >> 1]))).hash)->hpoint1,
            crypto_generichash_BYTES); // compare message itself
    }
    else
    {
        ret_gr_lev = memcmp(
            Shash,
            ((hash_point_p)((*(Layers_pointer[EXPONENT - 2].main_pointer[req_msg_first_nodeN >> 1]))).hash)->hpoint2,
            crypto_generichash_BYTES); // compare message itself
    }
    if (ret_gr_lev)
    {
        printf("not matched hash!\n");
        return 1;
    }
    else
    {
        printf("ground level matched!\n");
    }
    free(Shash);
    //-----------------------------------------//
    printf("\n");
    int Sret = 0;
    int rez = 0;
    for (int i = EXPONENT - 2; i >= 0; --i)
    {
        printf("I=%d\n", i);
        req_msg_first_nodeN >>= 1; // SHIFT
        printf("msg test N:%lu\n", req_msg_first_nodeN);
        if (i == 0)
        {
            printf("compare merkle root!\n");
            Shash =
                merge_2hashses(((hash_point_p)(*(Layers_pointer[i].main_pointer[i])).hpoint1)->hash, // take cur node
                               ((hash_point_p)(*(Layers_pointer[i].main_pointer[i])).hpoint2)->hash  // take cur node
                );
            rez = memcmp(Shash,
                         ((*(Layers_pointer[i].main_pointer[req_msg_first_nodeN])).hash), // take cur node
                         crypto_generichash_BYTES                                         // compare message itself
            );
            if (!rez)
                printf("merkle root verified!\n");
            free(Shash);
            Sret += rez;

            break;
        }
	size_t neighbor_number = (req_msg_first_nodeN % 2) ? (req_msg_first_nodeN-1) : (req_msg_first_nodeN+1); // some magic here
	//if (i%2) neighbor_number = !neighbor_number;
	if (neighbor_number) {
		Shash = merge_2hashses((*(Layers_pointer[i].main_pointer[neighbor_number])).hash,    // take cur node
                               (*(Layers_pointer[i].main_pointer[req_msg_first_nodeN ])).hash );// take neighbor node
	} else {
		Shash = merge_2hashses((*(Layers_pointer[i].main_pointer[req_msg_first_nodeN])).hash,    // take cur node
                               (*(Layers_pointer[i].main_pointer[neighbor_number ])).hash );// take neighbor node

		}

        	
         rez = memcmp(Shash,
                          ((*(Layers_pointer[i-1].main_pointer[req_msg_first_nodeN>>1])).hash), // take cur node
                         crypto_generichash_BYTES                                         // compare message itself
            );
        /*if (flip)
        {
            rez = memcmp(
                Shash,
                ((hash_point_p)((*(Layers_pointer[i - 1].main_pointer[req_msg_first_nodeN >> 1]))).hash)->hpoint1,
                crypto_generichash_BYTES); // compare message itself
        }
        else
        {
            rez = memcmp(
                Shash,
                ((hash_point_p)((*(Layers_pointer[i - 1].main_pointer[req_msg_first_nodeN >> 1]))).hash)->hpoint2,
                crypto_generichash_BYTES); // compare message itself
        }
	*/
        if (rez)
        {
            printf("validation failed in %d layer, %lu message\n", i, req_msg_first_nodeN);
            return 1;
        }
        free(Shash);
        Sret += rez;
    }
    printf("EXponen here:%zu\n", req_msg_first_nodeN);
    DumpHex(((hash_point_p)((*(Layers_pointer[req_msg_first_nodeN].main_pointer))->hash))->hash,
            crypto_generichash_BYTES);

    free(hash_msg2_f);
    free(hash_msg1_f);
    free(Shash2);
    Sret += ret_gr_lev;
    printf("RESULT %d\n", Sret);
    if (Sret)
        return 1;
    return 0;
}
