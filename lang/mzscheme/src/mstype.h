/*  "Welcome to Grame MidiShare(c)extension, version 1.6.22 Copyright(c)2001 Jacques Herry" */
/*                                                                                          */
/*                From      	*/
/*            Jacques Herry 	*/
/*           35, resid Corlay 	*/
/*           22300 Lannion 		*/
/*            France 			*/
/*  	Grame  MidiShare for schemers. */


#define sizeray(x) (sizeof(x)/sizeof(*x))

typedef struct {
  Scheme_Type type; 
  MidiEvPtr midi_ev_ptr;
} shNewEv;


static Scheme_Type ev_type;
static Scheme_Type ev_type_killed;

typedef struct {
  Scheme_Type type; 
  MidiFilterPtr filter_ptr;
} shFilter;

static Scheme_Type filter_type;
static Scheme_Type filter_type_killed ;


typedef struct {
  Scheme_Type type; 
  SmpteLocPtr smpte_loc_ptr;
} shSmpteLoc;
static Scheme_Type smpte_loc_type;
#define GET_SMPTE(SCM_smpte) ((shSmpteLoc *)scheme_struct_ref(SCM_smpte,6))
#define GET_SMPTE_TYPE(SCM_smpte)  (GET_SMPTE(SCM_smpte)->type)
#define SMPTEP(SCM_smpte)  ( GET_SMPTE_TYPE(SCM_smpte) == smpte_loc_type)
#define GET_SMPTE_PTR(SCM_smpte)  GET_SMPTE(SCM_smpte)->smpte_loc_ptr


typedef struct {
  Scheme_Type type; 
  MidiSeqPtr midi_seq_ptr;
} shNewSeq;
static Scheme_Type seq_type;
static Scheme_Type seq_type_killed;


typedef struct {
  Scheme_Type type; 
  PosPtr pos_ptr;
} shPos;
static Scheme_Type pos_type;

/* the macro for extract the pos struct from scm-struct */
#define GET_POS_PLAYER(SCM_pos_struct) ((shPos *)scheme_struct_ref(SCM_pos_struct,3))
#define GET_POS_TYPE(SCM_pos)  (GET_POS_PLAYER(SCM_pos)->type)
#define PLAYER_POSP(SCM_pos)  ( GET_POS_TYPE(SCM_pos) == pos_type)
#define GET_POS_PTR(SCM_pos)  GET_POS_PLAYER(SCM_pos)->pos_ptr


typedef struct {
  Scheme_Type type; 
  MidiFileInfosPtr midi_file_infos_ptr;
} shMidiFileInfos;

#define GET_INFOS_PLAYER(SCM_info_struct) ((shMidiFileInfos *)scheme_struct_ref(SCM_info_struct,4))
#define GET_INFOS_TYPE(SCM_info)  (GET_INFOS_PLAYER(SCM_info)->type)
#define PLAYER_INFOSP(SCM_info)  ( GET_INFOS_TYPE(SCM_info) == midi_file_infos_type)
#define GET_INFOS_PTR(SCM_info)  ((GET_INFOS_PLAYER(SCM_info))->midi_file_infos_ptr)


static Scheme_Type midi_file_infos_type;

/* I make correspondance with c-struct and scm-struct here */
static Scheme_Object *state_player_struct_type;
static Scheme_Object *pos_player_struct_type;
static Scheme_Object *infos_player_struct_type;

static Scheme_Object *smpte_loc_struct_type;
static Scheme_Object *sync_info_struct_type ;
