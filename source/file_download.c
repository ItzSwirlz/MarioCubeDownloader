// A lot of this code comes from Open Shop Channel's source code.
// Do not attribute the entire file to me.

#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

static int progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    printf("Progress: %d%%\n", (int)(dlnow*100/dltotal));
    return 0;
}

CURLcode download(char *url, char *outfile) {
    // FIXME: add USB support
    // TODO: error checking
    CURL *curl = curl_easy_init();
    CURLcode result = CURLE_FAILED_INIT;  // if the later condition is false an error happened

    if(curl) {
        FILE *fp = fopen(outfile, "arw");  // R/W access + be able to create the file if necessary

        // Curl options
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  // FIXME: fix ssl certificates. This disables the check which is unsafe
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);  // Disable the download timeout. The Wii's WiFi is so slow it will fail without this.
        curl_easy_setopt(curl, CURLOPT_URL, url);  // Set the URL
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  // Where to write the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  // FIXME: part of error checking
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);  // For percentage updates

        // Perform the action and catch the result!
        result = curl_easy_perform(curl);

        // Cleanup
        printf("Curl exited with code %d\n", result);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    return result;
}