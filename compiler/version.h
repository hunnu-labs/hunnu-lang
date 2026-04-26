/**
 * @file version.h
 * @brief Version constants using Mongolian women names (А-Я)
 */

#ifndef HUNNU_VERSION_H
#define HUNNU_VERSION_H

#include <stdint.h>

/**
 * Mongolian women names (А-Я) for versioning
 * Each name represents a milestone in the language development
 */
typedef enum {
    VERSION_AMAR = 0,      /**< 0.1.0 - Амар (peaceful) - MVP/initial release */
    VERSION_AMIN,          /**< 0.2.0 - Амин (truthful) */
    VERSION_GANDAI,          /**< 0.3.0 - Гандай (kind) */
    VERSION_MUNKHUUL,      /**< 0.4.0 - Мөнхөл (eternal) */
    VERSION_NAVCHAA,        /**< 0.5.0 - Навчаа (beautiful) */
    VERSION_OYUNA,          /**< 0.6.0 - Оюун (clever) */
    VERSION_UNDRAA,         /**< 0.7.0 - Ундраа (wisdom) */
    VERSION_HUNNU,         /**< 0.8.0 - Хүннү (human) */
    VERSION_TSETSEN,        /**< 0.9.0 - Цэцэг (flower) */
    VERSION_CHIMED,        /**< 1.0.0 - Чимэд (forever) */
    VERSION_SHINUR,        /**< 1.1.0 - Шинур (pearl) */
    VERSION_ERDENE,        /**< 1.2.0 - Эрдэнэ (jewel) - stable release */
    VERSION_COUNT
} HunnuVersion;

/** Current version - Амин (Basic features) */
#ifndef HUNNU_VERSION_CURRENT
#define HUNNU_VERSION_CURRENT VERSION_AMIN
#endif

/** Version names (Mongolian) */
static const char* version_names[] = {
    "Амар",
    "Амин", 
    "Гандай",
    "Мөнхөл",
    "Навчаа",
    "Оюун",
    "Ундраа",
    "Хүннү",
    "Цэцэг",
    "Чимэд",
    "Шинур",
    "Эрдэнэ"
};

/** Version descriptions */
static const char* version_descriptions[] = {
    "MVP - Анхны хувилбар",
    "Үндсэн функцууд",
    "Сайжруулсан хувилбар",
    "Тогтмол хувилбар",
    "Өнгөлөг хувилбар",
    "Ухаалаг хувилбар",
    "Төгс хувилбар",
    "Хүний хувилбар",
    "Цэцгийн хувилбар",
    "Тогтворол хувилбар",
    "Пүүзийн хувилбар",
    "Эрдэнэтийн хувилбар"
};

/** Version numbers */
static const uint32_t version_major_versions[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1
};

static const uint32_t version_minor_versions[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 0, 1, 2, 3
};

static const uint32_t version_patch_versions[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/**
 * @brief Gets current version name
 * @return Version name in Mongolian
 */
const char* version_get_name(void);

/**
 * @brief Gets current version description
 * @return Version description in Mongolian
 */
const char* version_get_description(void);

/**
 * @brief Gets major version number
 * @return Major version
 */
uint32_t version_get_major(void);

/**
 * @brief Gets minor version number
 * @return Minor version
 */
uint32_t version_get_minor(void);

/**
 * @brief Gets patch version number
 * @return Patch version
 */
uint32_t version_get_patch(void);

/**
 * @brief Gets full version string
 * @return Full version string (e.g., "0.2.0 (Амин)")
 */
const char* version_get_string(void);

#endif