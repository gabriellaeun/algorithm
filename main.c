#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int compare(uint8_t* first, uint8_t* second, size_t len);

int main()
{
    uint8_t* encoded = NULL;
 
    char* test_strings[] = {
                "test string",
                "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!\"£$%^&*()-=_+\\|,./<>?[]{}'#@~`¬\n",
                "test",
                "Hello world!",
                "This is a test string",
                "My name is Jeff",
                "Test",
                "tteesstt",
                "test",
                "ab",
                "Ω≈ç√∫˜µ≤≥÷",
                "ЁЂЃЄЅІЇЈЉЊЋЌЍЎЏАБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдежзийклмнопрстуфхцчшщъыьэюя",
                "If you're reading this, you've been in a coma for almost 20 years now. We're trying a new technique. We don't know where this message will end up in your dream, but we hope it works. Please wake up, we miss you.",
                "a",
                "aaaaaaaaaaaaaa",
                "\0",
                "Powerلُلُصّبُلُلصّبُررً ॣ ॣh ॣ ॣ冗",
                "When the sunlight strikes raindrops in the air, they act as a prism and form a rainbow. The rainbow is a division of white light into many beautiful colors. These take the shape of a long round arch, with its path high above, and its two ends apparently beyond the horizon. There is , according to legend, a boiling pot of gold at one end. People look, but no one ever finds it. When a man looks for something beyond his reach, his friends say he is looking for the pot of gold at the end of the rainbow. Throughout the centuries people have explained the rainbow in various ways. Some have accepted it as a miracle without physical explanation. To the Hebrews it was a token that there would be no more universal floods. The Greeks used to imagine that it was a sign from the gods to foretell war or heavy rain. The Norsemen considered the rainbow as a bridge over which the gods passed from earth to their home in the sky. Others have tried to explain the phenomenon physically. Aristotle thought that the rainbow was caused by reflection of the sun's rays by the rain. Since then physicists have found that it is not reflection, but refraction by the raindrops which causes the rainbows. Many complicated ideas about the rainbow have been formed. The difference in the rainbow depends considerably upon the size of the drops, and the width of the colored band increases as the size of the drops increases. The actual primary rainbow observed is said to be the effect of super-imposition of a number of bows. If the red of the second bow falls upon the green of the first, the result is to give a bow with an abnormally wide yellow band, since red and green light when mixed form yellow. This is a very common type of bow, one showing mainly red and yellow, with little or no green or "
    }; /* A series of horrible strings that try and break the compression */

    size_t successes = 0;
    size_t failures = 0;
    size_t test_count = sizeof(test_strings) / sizeof(test_strings[0]);

    for (size_t i = 0; i < test_count; i++) {
        printf("\nEncoding string %zu...", i);
        fflush(stdout);

        if (huffman_encode((uint8_t*)test_strings[i], &encoded, strlen(test_strings[i]) + 1) != EXIT_SUCCESS) {
            fprintf(stderr, "\nError: Failed to encode string %zu!\n", i);
            failures++;
            continue;
        }

        printf("Done!\nAttempting to decode...");
        fflush(stdout);

        free(encoded);

        successes++;
    }

    printf("Results:\n\nTests completed: %zu\nSuccessful tests: %zu (%.0f%%)\n", test_count, successes, (float)successes / test_count);

    return 0;
}

int compare(uint8_t* first, uint8_t* second, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (first[i] < second[i]) {
            return -1;
        }
        else if (first[i] > second[i]) {
            return 1;
        }
    }

    return 0;
}
