#include "ds.h"

#include <string.h>

bool ds_string_from_cstr(DsString *string, const char *source)
{
    size_t length;

    if (string == NULL || source == NULL) {
        return false;
    }
    length = strlen(source);
    if (length > DS_STRING_MAX) {
        return false;
    }
    memcpy(string->data, source, length + 1);
    string->length = length;
    return true;
}

int ds_string_find_naive(const DsString *text, const DsString *pattern)
{
    size_t start;

    if (text == NULL || pattern == NULL ||
        text->length > DS_STRING_MAX || pattern->length > DS_STRING_MAX) {
        return -1;
    }
    if (pattern->length == 0) {
        return 0;
    }
    if (pattern->length > text->length) {
        return -1;
    }
    for (start = 0; start <= text->length - pattern->length; ++start) {
        size_t offset = 0;
        while (offset < pattern->length &&
               text->data[start + offset] == pattern->data[offset]) {
            ++offset;
        }
        if (offset == pattern->length) {
            return (int)start;
        }
    }
    return -1;
}

int ds_string_find_kmp(const DsString *text, const DsString *pattern)
{
    size_t failure[DS_STRING_MAX];
    size_t i;
    size_t j;
    size_t matched = 0;

    if (text == NULL || pattern == NULL ||
        text->length > DS_STRING_MAX || pattern->length > DS_STRING_MAX) {
        return -1;
    }
    if (pattern->length == 0) {
        return 0;
    }
    if (pattern->length > text->length) {
        return -1;
    }

    /* failure[i] 是模式串前缀的回退位置，避免失配后从头比较。 */
    failure[0] = 0;
    for (i = 1, j = 0; i < pattern->length; ++i) {
        while (j > 0 && pattern->data[i] != pattern->data[j]) {
            j = failure[j - 1];
        }
        if (pattern->data[i] == pattern->data[j]) {
            ++j;
        }
        failure[i] = j;
    }

    for (i = 0; i < text->length; ++i) {
        while (matched > 0 && text->data[i] != pattern->data[matched]) {
            matched = failure[matched - 1];
        }
        if (text->data[i] == pattern->data[matched]) {
            ++matched;
        }
        if (matched == pattern->length) {
            return (int)(i + 1 - pattern->length);
        }
    }
    return -1;
}
