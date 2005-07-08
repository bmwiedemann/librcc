#ifndef _LIBRCC_H
#define _LIBRCC_H

/*******************************************************************************
***************************** Global Defines ***********************************
*******************************************************************************/

#define RCC_MAX_CHARSETS 16
#define RCC_MAX_ENGINES 5
#define RCC_MAX_LANGUAGES 64
#define RCC_MAX_ALIASES 64
#define RCC_MAX_CLASSES 16

#define RCC_MAX_ERRORS 3

#define RCC_MAX_CHARSET_CHARS 16
#define RCC_MAX_LANGUAGE_CHARS 16
#define RCC_MAX_VARIABLE_CHARS 16


/* ID's */
typedef char rcc_language_id;
typedef char rcc_alias_id;
typedef char rcc_charset_id;
typedef char rcc_engine_id;

typedef int rcc_class_id;

/* Opaque Pointer's */
typedef struct rcc_context_t *rcc_context;
typedef struct rcc_engine_context_t *rcc_engine_context;
typedef struct rcc_language_config_t *rcc_language_config;
typedef const struct rcc_class_t *rcc_class_ptr;

#ifdef __cplusplus
extern "C" {
#endif

int rccInit();
void rccFree();

/*******************************************************************************
**************************** Initialization ************************************
*******************************************************************************/
typedef unsigned int rcc_init_flags;
#define RCC_NO_DEFAULT_CONFIGURATION 1
rcc_context rccCreateContext(const char *locale_variable, unsigned int max_languages, unsigned int max_classes, rcc_class_ptr defclasses, rcc_init_flags flags);
int rccInitDefaultContext(const char *locale_variable, unsigned int max_languages, unsigned int max_classes, rcc_class_ptr defclasses, rcc_init_flags flags);
void rccFreeContext(rcc_context ctx);

typedef unsigned int rcc_db4_flags;
int rccInitDb4(rcc_context ctx, const char *name, rcc_db4_flags flags);

int rccLockConfiguration(rcc_context ctx, unsigned int lock_code);
int rccUnlockConfiguration(rcc_context ctx, unsigned int lock_code);

/*******************************************************************************
******************* Altering Language Configuaration ***************************
*******************************************************************************/
typedef const char *rcc_charset;
typedef rcc_charset rcc_charset_list[RCC_MAX_CHARSETS+1];

/* Engines */
typedef void *rcc_engine_internal;
typedef rcc_engine_internal (*rcc_engine_init_function)(rcc_engine_context ctx);
typedef rcc_charset_id (*rcc_engine_function)(rcc_engine_context ctx, const char *buf, int len);
typedef void (*rcc_engine_free_function)(rcc_engine_context ctx);

struct rcc_engine_t {
    const char *title;
    rcc_engine_init_function init_func;
    rcc_engine_free_function free_func;
    rcc_engine_function func;
    rcc_charset_list charsets;
};
typedef struct rcc_engine_t rcc_engine;
typedef rcc_engine *rcc_engine_ptr;
typedef rcc_engine_ptr rcc_engine_list[RCC_MAX_ENGINES+1];

/* Language */
struct rcc_language_t {
    const char *sn;
    rcc_charset_list charsets;
    rcc_engine_list engines;
};
typedef struct rcc_language_t rcc_language;
typedef rcc_language *rcc_language_ptr;
typedef rcc_language_ptr rcc_language_list[RCC_MAX_LANGUAGES+1];

/* Alias */
struct rcc_language_alias_t {
    const char *alias;
    const char *lang;
};
typedef struct rcc_language_alias_t rcc_language_alias;
typedef rcc_language_alias *rcc_language_alias_ptr;
typedef rcc_language_alias_ptr rcc_language_alias_list[RCC_MAX_ALIASES+1];

struct rcc_language_name_t {
    const char *sn;
    const char *name;
};
typedef struct rcc_language_name_t rcc_language_name;

rcc_language_id rccRegisterLanguage(rcc_context ctx, rcc_language *language);
rcc_charset_id rccLanguageRegisterCharset(rcc_language *language, rcc_charset charset);
rcc_engine_id rccLanguageRegisterEngine(rcc_language *language, rcc_engine *engine);
rcc_alias_id rccRegisterLanguageAlias(rcc_context ctx, rcc_language_alias *alias);

/*******************************************************************************
************************ Altering Configuaration *******************************
*******************************************************************************/
typedef enum rcc_class_type_t {
    RCC_CLASS_INVALID = 0,
    RCC_CLASS_STANDARD,
    RCC_CLASS_KNOWN,
    RCC_CLASS_FS
} rcc_class_type;

struct rcc_class_default_charset_t {
    const char *lang;
    const char *charset;
};
typedef const struct rcc_class_default_charset_t rcc_class_default_charset;

struct rcc_class_t {
    const char *name;
    const rcc_class_type class_type;
    const char *defvalue; /* locale variable name or parrent name */
    rcc_class_default_charset *defcharset;
    const char *fullname;
};
typedef const struct rcc_class_t rcc_class;
typedef rcc_class_ptr rcc_class_list[RCC_MAX_CLASSES+1];

rcc_class_id rccRegisterClass(rcc_context ctx, rcc_class *cl);
rcc_class_type rccGetClassType(rcc_context ctx, rcc_class_id class_id);

/*******************************************************************************
************************ Altering Configuaration *******************************
*******************************************************************************/
typedef int rcc_option_value;
typedef enum rcc_option_t {
    RCC_LEARNING_MODE = 0,
    RCC_AUTODETECT_FS_TITLES,
    RCC_AUTODETECT_FS_NAMES,
    RCC_CONFIGURED_LANGUAGES_ONLY,
    RCC_MAX_OPTIONS
} rcc_option;

struct rcc_option_name_t {
    rcc_option option;
    const char *name;

    const char **subnames;
};
typedef struct rcc_option_name_t rcc_option_name;

/* lng.c */
const char *rccGetLanguageName(rcc_context ctx, rcc_language_id language_id);
rcc_language_id rccGetLanguageByName(rcc_context ctx, const char *name);
rcc_language_id rccGetRealLanguage(rcc_context ctx, rcc_language_id language_id);
const char *rccGetRealLanguageName(rcc_context ctx, rcc_language_id language_id);
rcc_language_id rccGetSelectedLanguage(rcc_context ctx);
const char *rccGetSelectedLanguageName(rcc_context ctx);
rcc_language_id rccGetCurrentLanguage(rcc_context ctx);
const char *rccGetCurrentLanguageName(rcc_context ctx);

int rccSetLanguage(rcc_context ctx, rcc_language_id language_id);
int rccSetLanguageByName(rcc_context ctx, const char *name);

/* opt.c */
rcc_option_value rccGetOption(rcc_context ctx, rcc_option option);
int rccOptionIsDefault(rcc_context ctx, rcc_option option);
int rccOptionSetDefault(rcc_context ctx, rcc_option option);
int rccSetOption(rcc_context ctx, rcc_option option, rcc_option_value value);

/* lngconfig.c */

const char *rccConfigGetEngineName(rcc_language_config config, rcc_engine_id engine_id);
const char *rccConfigGetCharsetName(rcc_language_config config, rcc_charset_id charset_id);
const char *rccConfigGetAutoCharsetName(rcc_language_config config, rcc_charset_id charset_id);
rcc_engine_id rccConfigGetEngineByName(rcc_language_config config, const char *name);
rcc_charset_id rccConfigGetCharsetByName(rcc_language_config config, const char *name);
rcc_charset_id rccConfigGetAutoCharsetByName(rcc_language_config config, const char *name);

rcc_language_config rccCheckConfig(rcc_context ctx, rcc_language_id language_id);
rcc_language_config rccGetConfig(rcc_context ctx, rcc_language_id language_id);
rcc_language_config rccGetConfigByName(rcc_context ctx, const char *name);
rcc_language_config rccGetCurrentConfig(rcc_context ctx);

rcc_engine_id rccConfigGetSelectedEngine(rcc_language_config config);
const char *rccConfigGetSelectedEngineName(rcc_language_config config);
rcc_engine_id rccConfigGetCurrentEngine(rcc_language_config config);
const char *rccConfigGetCurrentEngineName(rcc_language_config config);
rcc_charset_id rccConfigGetSelectedCharset(rcc_language_config config, rcc_class_id class_id);
const char *rccConfigGetSelectedCharsetName(rcc_language_config config, rcc_class_id class_id);
rcc_charset_id rccConfigGetCurrentCharset(rcc_language_config config, rcc_class_id class_id);
const char *rccConfigGetCurrentCharsetName(rcc_language_config config, rcc_class_id class_id);

int rccConfigSetEngine(rcc_language_config config, rcc_engine_id engine_id);
int rccConfigSetCharset(rcc_language_config config, rcc_class_id class_id, rcc_charset_id charset_id);
int rccConfigSetEngineByName(rcc_language_config config, const char *name);
int rccConfigSetCharsetByName(rcc_language_config config, rcc_class_id class_id, const char *name);

rcc_charset_id rccConfigGetLocaleCharset(rcc_language_config config, const char *locale_variable);

/* curconfig.c */
const char *rccGetEngineName(rcc_context ctx, rcc_engine_id engine_id);
const char *rccGetCharsetName(rcc_context ctx, rcc_charset_id charset_id);
const char *rccGetAutoCharsetName(rcc_context ctx, rcc_charset_id charset_id);

rcc_engine_id rccGetEngineByName(rcc_context ctx, const char *name);
rcc_charset_id rccGetCharsetByName(rcc_context ctx, const char *name);
rcc_charset_id rccGetAutoCharsetByName(rcc_context ctx, const char *name);

rcc_engine_id rccGetSelectedEngine(rcc_context ctx);
const char *rccGetSelectedEngineName(rcc_context ctx);
rcc_engine_id rccGetCurrentEngine(rcc_context ctx);
const char *rccGetCurrentEngineName(rcc_context ctx);
rcc_charset_id rccGetSelectedCharset(rcc_context ctx, rcc_class_id class_id);
const char *rccGetSelectedCharsetName(rcc_context ctx, rcc_class_id class_id);
rcc_charset_id rccGetCurrentCharset(rcc_context ctx, rcc_class_id class_id);
const char *rccGetCurrentCharsetName(rcc_context ctx, rcc_class_id class_id);

int rccSetEngine(rcc_context ctx, rcc_engine_id engine_id);
int rccSetCharset(rcc_context ctx, rcc_class_id class_id, rcc_charset_id charset_id);
int rccSetEngineByName(rcc_context ctx, const char *name);
int rccSetCharsetByName(rcc_context ctx, rcc_class_id class_id, const char *name);

rcc_charset_id rccGetLocaleCharset(rcc_context ctx, const char *locale_variable);

/*******************************************************************************
************************ Language Configuaration *******************************
*******************************************************************************/
/* rcclist.c */
rcc_language_ptr *rccGetLanguageList(rcc_context ctx);
rcc_charset *rccGetCharsetList(rcc_context ctx, rcc_language_id language_id);
rcc_engine_ptr *rccGetEngineList(rcc_context ctx, rcc_language_id language_id);
rcc_charset *rccGetCurrentCharsetList(rcc_context ctx);
rcc_engine_ptr *rccGetCurrentEngineList(rcc_context ctx);
rcc_charset *rccGetCurrentAutoCharsetList(rcc_context ctx);
rcc_class_ptr *rccGetClassList(rcc_context ctx);

/*******************************************************************************
************************ Default Configuaration ********************************
*******************************************************************************/

/* rccconfig.c */
const char *rccGetOptionName(rcc_option option);
const char *rccGetOptionFullName(rcc_option option);
rcc_option_value rccGetOptionDefaultValue(rcc_option option);
const char *rccGetLanguageFullName(const char *lang);


/*******************************************************************************
************************ RCC_STRING Manipulations ******************************
*******************************************************************************/
/* string.c */
typedef char *rcc_string;

size_t rccStringCheck(const char *str);
size_t rccStringSizedCheck(const char *str, size_t len);

rcc_language_id rccStringGetLanguage(const rcc_string str);
const char *rccStringGetString(const rcc_string str);
char *rccStringExtractString(const rcc_string str);

const char *rccGetString(const char *str);
const char *rccSizedGetString(const char *str, size_t len, size_t *rlen);

int rccStringCmp(const char *str1, const char *str2);
int rccStringNCmp(const char *str1, const char *str2, size_t n);
int rccStringCaseCmp(const char *str1, const char *str2);
int rccStringNCaseCmp(const char *str1, const char *str2, size_t n);

/*******************************************************************************
******************************** Recoding **************************************
*******************************************************************************/

/* recode.c */
rcc_string rccFrom(rcc_context ctx, rcc_class_id class_id, const char *buf, size_t len, size_t *rlen);
char *rccTo(rcc_context ctx, rcc_class_id class_id, const rcc_string buf, size_t len, size_t *rlen);
char *rccRecode(rcc_context ctx, rcc_class_id from, rcc_class_id to, const char *buf, size_t len, size_t *rlen);
char *rccFS(rcc_context ctx, rcc_class_id from, rcc_class_id to, const char *fspath, const char *path, const char *filename);

/*******************************************************************************
******************************** Options ***************************************
*******************************************************************************/

/* xml.c */
int rccSave(rcc_context ctx, const char *name);
int rccLoad(rcc_context ctx, const char *name);

#ifdef __cplusplus
}
#endif

#endif /* _LIBRCC_H */
