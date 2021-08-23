/**
 * A convenience set mapping frequency to midi notes
 */

#include <map>
#include <iostream>

using namespace std;

#define LOWEST_MIDI 36
#define HIGHEST_MIDI 72

static map<int, float> midiToFreq = {
    { 36, 65.41f},
    { 37, 69.30f},
    { 38, 73.42f},
    { 39, 77.78f},
    { 40, 82.41f},
    { 41, 87.31f},
    { 42, 92.50f},
    { 43, 98.00f},
    { 44, 103.83f},
    { 45, 110.00f},
    { 46, 116.54f},
    { 47, 123.47f},
    { 48, 130.81f},
    { 49, 138.59f},
    { 50, 146.83f},
    { 51, 155.56f},
    { 52, 164.81f},
    { 53, 174.61f},
    { 54, 185.00f},
    { 55, 196.00f},
    { 56, 207.65f},
    { 57, 220.00f},
    { 58, 233.08f},
    { 59, 246.94f},
    { 60, 261.63f},
    { 61, 277.18f},
    { 62, 293.66f},
    { 63, 311.13f},
    { 64, 329.63f},
    { 65, 349.23f},
    { 66, 369.99f},
    { 67, 392.00f},
    { 68, 415.30f},
    { 69, 440.00f},
    { 70, 466.16f},
    { 71, 493.88f},
    { 72, 526.25f}
};

// binary search to guess a midi note to correspond to your frequency
static int findMidiNote(float freq, float allowedError)
{
    if (freq < 65.41 || freq > 526.0)
        return -1;
    if (freq > 165.0f)
        allowedError *= 2;
    int lowKey = LOWEST_MIDI;
    int highKey = HIGHEST_MIDI;
    while (midiToFreq[lowKey] <= midiToFreq[highKey]) {
        int mid = (lowKey + highKey) / 2;
        float midFreq = midiToFreq[mid];
//        cout << "low: " << lowKey << ", high: " << highKey << endl;
//        cout << "mid: " << mid << ", midFreq: " << midFreq << endl;
//        cout << "freq < midFreq?: " << (freq < midFreq) << endl;
        if ((freq <= midFreq && freq >= (midFreq - allowedError)) ||
            (freq >= midFreq && freq <= (midFreq + allowedError))) { // the equivalent of 'I'm gonna call that close enough'
            return mid;
        }
        else {
            if (midFreq < freq) {
//                cout << "mid too low, moving up" << endl;
                lowKey = mid + 1;
            }
            else {
//                cout << "mid too high, moving down" << endl;
                highKey = mid - 1;
            }
        }
//        cout << "   ...   " << endl;
    }
    return -1; // didn't find anything good (within the table)
}

//int main()
//{
//    while (true) {
//        cout << "Input frequency: ";
//        float input;
//        cin >> input;
//        int foundNote = findMidiNote(input, 3.0f);
//        cout << "Found midi note: " << foundNote << endl;
//        cout << "* * * * * * * done * * * * * * *" << endl;
//    }
//}
