#include "sjson/main.h"
#include <assert.h>
#include <stdio.h>

#define SAMPLE_JSON_DATA \
    "{\n" \
    "  \"id\": 12345,\n" \
    "  \"name\": \"Example\",\n" \
    "  \"active\": true,\n" \
    "  \"score\": 98.76,\n" \
    "  \"scientific\": 1.23e4,\n" \
    "  \"tags\": [\"alpha\", \"beta\", \"gamma\"],\n" \
    "  \"meta\": {\n" \
    "    \"created\": \"2025-06-14T12:00:00Z\",\n" \
    "    \"updated\": null,\n" \
    "    \"flags\": {\n" \
    "      \"archived\": false,\n" \
    "      \"featured\": true\n" \
    "    }\n" \
    "  },\n" \
    "  \"items\": [\n" \
    "    {\n" \
    "      \"type\": \"book\",\n" \
    "      \"price\": 19.99,\n" \
    "      \"available\": true\n" \
    "    },\n" \
    "    {\n" \
    "      \"type\": \"pen\",\n" \
    "      \"price\": 1.5,\n" \
    "      \"available\": false\n" \
    "    }\n" \
    "  ]\n" \
    "}"



void test__sjson__parse() {
    printf("TEST STARTED [%s::%s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
    const char* json_payload = SAMPLE_JSON_DATA;
    SJSON_DECL_ERR_V(e);
    sjson__parse(json_payload, e);
    printf("TESTED\n");
}

int main() {
    test__sjson__parse();
    return 0;
}
