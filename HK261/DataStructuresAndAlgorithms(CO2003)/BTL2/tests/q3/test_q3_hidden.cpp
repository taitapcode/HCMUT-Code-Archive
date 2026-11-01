// Extra Q3 cases modelled on what a hidden grader typically checks:
// the two worked examples in the spec, (searchKey, stopId) ordering,
// page arithmetic relative to the first match, page/pageSize edges,
// duplicate handling, catalog consistency after removals, a randomized
// differential run against a sorted reference, and deep-page cost.
#include "tests/TestUtils.h"
#include "BKMove.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <string>
#include <vector>

namespace {

StopInfo stop(const std::string& id, const std::string& name, const std::string& key) {
    return StopInfo{id, name, key};
}

std::vector<std::string> ids(const std::vector<StopInfo>& rows) {
    std::vector<std::string> out;
    for (const StopInfo& row : rows) out.push_back(row.stopId);
    return out;
}

// The seven stops of worked example 1, added in the spec's unsorted order.
void exampleCatalog(BKMove& app) {
    CHECK(app.addStop(stop("S007", "Cho Lon", "cho lon")));
    CHECK(app.addStop(stop("S005", "Ben xe Mien Tay", "ben xe mien tay")));
    CHECK(app.addStop(stop("S001", "Ben Thanh", "ben thanh")));
    CHECK(app.addStop(stop("S012", "Dai hoc Bach Khoa", "dai hoc bach khoa")));
    CHECK(app.addStop(stop("S004", "Ben xe Mien Dong", "ben xe mien dong")));
    CHECK(app.addStop(stop("S009", "Ben xe Cho Lon", "ben xe cho lon")));
    CHECK(app.addStop(stop("S021", "Cong vien Tao Dan", "cong vien tao dan")));
}

// Catalog whose keys are spread over the alphabet, so a prefix taken from the
// middle of the key space starts at a rank far from 0.
void spreadCatalog(BKMove& app) {
    CHECK(app.addStop(stop("A1", "An Giang", "an giang")));
    CHECK(app.addStop(stop("B1", "Ben Thanh", "ben thanh")));
    CHECK(app.addStop(stop("C1", "Ca Mau", "ca mau")));
    CHECK(app.addStop(stop("D1", "Da Lat", "da lat")));
    CHECK(app.addStop(stop("H1", "Ha Noi", "ha noi")));
    CHECK(app.addStop(stop("H2", "Hai Phong", "hai phong")));
    CHECK(app.addStop(stop("H3", "Hue", "hue")));
    CHECK(app.addStop(stop("N1", "Nha Trang", "nha trang")));
    CHECK(app.addStop(stop("S1", "Sai Gon", "sai gon")));
    CHECK(app.addStop(stop("V1", "Vung Tau", "vung tau")));
}

// Brute-force model: filter, sort by (searchKey, stopId), slice the page.
std::vector<StopInfo> referencePage(const std::vector<StopInfo>& catalog,
                                    const std::string& prefix, int page, int pageSize) {
    std::vector<StopInfo> rows;
    if (page < 0 || pageSize <= 0) return rows;
    for (const StopInfo& row : catalog) {
        if (row.searchKey.compare(0, prefix.size(), prefix) == 0) rows.push_back(row);
    }
    std::sort(rows.begin(), rows.end(), [](const StopInfo& a, const StopInfo& b) {
        if (a.searchKey != b.searchKey) return a.searchKey < b.searchKey;
        return a.stopId < b.stopId;
    });
    const long long start = static_cast<long long>(page) * pageSize;
    if (start >= static_cast<long long>(rows.size())) return {};
    const long long end = std::min<long long>(start + pageSize, static_cast<long long>(rows.size()));
    return std::vector<StopInfo>(rows.begin() + start, rows.begin() + end);
}

void checkSameRows(const std::vector<StopInfo>& got, const std::vector<StopInfo>& expected) {
    CHECK(got.size() == expected.size());
    for (std::size_t i = 0; i < got.size(); ++i) {
        CHECK(got[i].stopId == expected[i].stopId);
        CHECK(got[i].displayName == expected[i].displayName);
        CHECK(got[i].searchKey == expected[i].searchKey);
    }
}

unsigned nextRandom(unsigned& seed) {
    seed = seed * 1103515245u + 12345u;
    return seed >> 8;
}

}  // namespace

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q3 - Bus Stop Finder extra/hidden-style");

    suite.add("worked example 1: prefix pages", [] {
        BKMove app;
        exampleCatalog(app);
        CHECK(app.getStopCount() == 7);
        checkSameRows(app.findStops("ben", 0, 2), {stop("S001", "Ben Thanh", "ben thanh"),
                                                   stop("S009", "Ben xe Cho Lon", "ben xe cho lon")});
        checkSameRows(app.findStops("ben", 1, 2), {stop("S004", "Ben xe Mien Dong", "ben xe mien dong"),
                                                   stop("S005", "Ben xe Mien Tay", "ben xe mien tay")});
        CHECK(app.findStops("ben", 2, 2).empty());
        checkSameRows(app.findStops("ben xe", 0, 2), {stop("S009", "Ben xe Cho Lon", "ben xe cho lon"),
                                                      stop("S004", "Ben xe Mien Dong", "ben xe mien dong")});
        checkSameRows(app.findStops("ben xe", 1, 2), {stop("S005", "Ben xe Mien Tay", "ben xe mien tay")});
        CHECK(app.findStops("ben xe", 2, 2).empty());
    });

    suite.add("worked example 2: duplicate searchKey ordered by stopId", [] {
        BKMove app;
        CHECK(app.addStop(stop("S010", "Ben Thanh", "ben thanh")));
        CHECK(app.addStop(stop("S001", "Ben Thanh", "ben thanh")));
        CHECK(!app.addStop(stop("S001", "Other", "other")));
        CHECK(app.getStopCount() == 2);
        checkSameRows(app.findStops("ben", 0, 10), {stop("S001", "Ben Thanh", "ben thanh"),
                                                    stop("S010", "Ben Thanh", "ben thanh")});
        checkSameRows(app.findStops("ben", 1, 1), {stop("S010", "Ben Thanh", "ben thanh")});
        StopInfo out;
        CHECK(app.findStop("S001", out) && out.displayName == "Ben Thanh");
        CHECK(app.findStop("S010", out) && out.displayName == "Ben Thanh");
        CHECK(out.stopId == "S010");
    });

    suite.add("empty prefix lists the whole catalog in (searchKey, stopId) order", [] {
        BKMove app;
        spreadCatalog(app);
        checkSameRows(app.findStops("", 0, 100), {stop("A1", "An Giang", "an giang"),
                                                  stop("B1", "Ben Thanh", "ben thanh"),
                                                  stop("C1", "Ca Mau", "ca mau"),
                                                  stop("D1", "Da Lat", "da lat"),
                                                  stop("H1", "Ha Noi", "ha noi"),
                                                  stop("H2", "Hai Phong", "hai phong"),
                                                  stop("H3", "Hue", "hue"),
                                                  stop("N1", "Nha Trang", "nha trang"),
                                                  stop("S1", "Sai Gon", "sai gon"),
                                                  stop("V1", "Vung Tau", "vung tau")});
        checkSameRows(app.findStops("", 1, 3), {stop("D1", "Da Lat", "da lat"),
                                                stop("H1", "Ha Noi", "ha noi"),
                                                stop("H2", "Hai Phong", "hai phong")});
        checkSameRows(app.findStops("", 2, 3), {stop("H3", "Hue", "hue"),
                                                stop("N1", "Nha Trang", "nha trang"),
                                                stop("S1", "Sai Gon", "sai gon")});
        checkSameRows(app.findStops("", 3, 3), {stop("V1", "Vung Tau", "vung tau")});
        CHECK(app.findStops("", 4, 3).empty());
    });

    suite.add("pages are relative to the first match, not to the catalog", [] {
        BKMove app;
        spreadCatalog(app);
        // "ha" matches ranks 4..5; page 1 must skip one match, not one catalog row.
        checkSameRows(app.findStops("ha", 0, 2), {stop("H1", "Ha Noi", "ha noi"),
                                                  stop("H2", "Hai Phong", "hai phong")});
        CHECK(app.findStops("ha", 1, 2).empty());
        checkSameRows(app.findStops("ha", 0, 1), {stop("H1", "Ha Noi", "ha noi")});
        checkSameRows(app.findStops("ha", 1, 1), {stop("H2", "Hai Phong", "hai phong")});
        CHECK(app.findStops("ha", 2, 1).empty());
        // A prefix whose only match sits deep in the catalog.
        checkSameRows(app.findStops("n", 0, 5), {stop("N1", "Nha Trang", "nha trang")});
        checkSameRows(app.findStops("n", 0, 1), {stop("N1", "Nha Trang", "nha trang")});
        checkSameRows(app.findStops("v", 0, 5), {stop("V1", "Vung Tau", "vung tau")});
        checkSameRows(app.findStops("s", 0, 5), {stop("S1", "Sai Gon", "sai gon")});
        checkSameRows(app.findStops("d", 1, 1), {});
    });

    suite.add("prefix that matches nothing between existing keys", [] {
        BKMove app;
        spreadCatalog(app);
        CHECK(app.findStops("bena", 0, 10).empty());
        CHECK(app.findStops("haz", 0, 10).empty());
        CHECK(app.findStops("ha ", 0, 10).size() == 1);  // only "ha noi"
        CHECK(app.findStops("ha n", 0, 10).size() == 1);
        CHECK(app.findStops("hai ", 0, 10).size() == 1);  // only "hai phong"
        CHECK(app.findStops("hai p", 0, 10).size() == 1);
        CHECK(app.findStops("hai q", 0, 10).empty());
        CHECK(app.findStops("z", 0, 10).empty());
        CHECK(app.findStops("aaa", 0, 10).empty());
    });

    suite.add("prefix shorter, equal and longer than a searchKey", [] {
        BKMove app;
        CHECK(app.addStop(stop("B1", "Ben Thanh", "ben thanh")));
        CHECK(app.addStop(stop("B2", "Ben Thanh 2", "ben thanh 2")));
        CHECK(app.addStop(stop("B3", "Ben Thanh X", "ben thanh x")));
        checkSameRows(app.findStops("ben than", 0, 10), {stop("B1", "Ben Thanh", "ben thanh"),
                                                         stop("B2", "Ben Thanh 2", "ben thanh 2"),
                                                         stop("B3", "Ben Thanh X", "ben thanh x")});
        checkSameRows(app.findStops("ben thanh", 0, 10), {stop("B1", "Ben Thanh", "ben thanh"),
                                                          stop("B2", "Ben Thanh 2", "ben thanh 2"),
                                                          stop("B3", "Ben Thanh X", "ben thanh x")});
        checkSameRows(app.findStops("ben thanh 2", 0, 10), {stop("B2", "Ben Thanh 2", "ben thanh 2")});
        checkSameRows(app.findStops("ben thanh x", 0, 10), {stop("B3", "Ben Thanh X", "ben thanh x")});
        CHECK(app.findStops("ben thanh 3", 0, 10).empty());
        CHECK(app.findStops("ben thanh xx", 0, 10).empty());
    });

    suite.add("page and pageSize edge values", [] {
        BKMove app;
        spreadCatalog(app);
        CHECK(app.findStops("", 0, 100).size() == 10);
        CHECK(app.findStops("", 0, 10).size() == 10);
        CHECK(app.findStops("", 9, 1).size() == 1 && app.findStops("", 9, 1)[0].stopId == "V1");
        CHECK(app.findStops("", 10, 1).empty());
        CHECK(app.findStops("", 11, 1).empty());
        CHECK(app.findStops("", -1, 5).empty());
        CHECK(app.findStops("", -1000, 5).empty());
        CHECK(app.findStops("", 0, 0).empty());
        CHECK(app.findStops("", 0, -1).empty());
        CHECK(app.findStops("", 2, 0).empty());
        CHECK(app.findStops("", INT_MAX, INT_MAX).empty());
        CHECK(app.findStops("", INT_MAX / 2, 2).empty());
        CHECK(app.findStops("", INT_MAX, 1).empty());
        CHECK(app.findStops("", 0, INT_MAX).size() == 10);
        // The catalog is untouched by all of the above.
        CHECK(app.getStopCount() == 10);
    });

    suite.add("stopId tie-break is lexicographic, not numeric", [] {
        BKMove app;
        CHECK(app.addStop(stop("S2", "Ben Thanh", "ben thanh")));
        CHECK(app.addStop(stop("S20", "Ben Thanh", "ben thanh")));
        CHECK(app.addStop(stop("S10", "Ben Thanh", "ben thanh")));
        CHECK(app.addStop(stop("S1", "Ben Thanh", "ben thanh")));
        const std::vector<std::string> expected = {"S1", "S10", "S2", "S20"};
        CHECK(ids(app.findStops("ben", 0, 4)) == expected);
        CHECK(ids(app.findStops("ben", 1, 2)) == (std::vector<std::string>{"S2", "S20"}));
        CHECK(ids(app.findStops("ben", 2, 2)).empty());
    });

    suite.add("removeStop drops the stop from every page and reinsert restores order", [] {
        BKMove app;
        exampleCatalog(app);
        CHECK(app.removeStop("S009"));
        CHECK(!app.removeStop("S009"));
        CHECK(app.getStopCount() == 6);
        checkSameRows(app.findStops("ben", 0, 2), {stop("S001", "Ben Thanh", "ben thanh"),
                                                   stop("S004", "Ben xe Mien Dong", "ben xe mien dong")});
        checkSameRows(app.findStops("ben", 1, 2), {stop("S005", "Ben xe Mien Tay", "ben xe mien tay")});
        checkSameRows(app.findStops("ben xe", 0, 5), {stop("S004", "Ben xe Mien Dong", "ben xe mien dong"),
                                                      stop("S005", "Ben xe Mien Tay", "ben xe mien tay")});
        CHECK(app.findStops("ben xe cho", 0, 5).empty());
        StopInfo out{"sentinel", "keep", "keep"};
        CHECK(!app.findStop("S009", out) && out.stopId == "sentinel");
        CHECK(app.addStop(stop("S009", "Ben xe Cho Lon", "ben xe cho lon")));
        checkSameRows(app.findStops("ben", 0, 2), {stop("S001", "Ben Thanh", "ben thanh"),
                                                   stop("S009", "Ben xe Cho Lon", "ben xe cho lon")});
        CHECK(app.getStopCount() == 7);
    });

    suite.add("findStop copies every field and leaves out untouched on failure", [] {
        BKMove app;
        exampleCatalog(app);
        StopInfo out{"sentinel", "keep", "keep"};
        CHECK(app.findStop("S012", out));
        CHECK(out.stopId == "S012");
        CHECK(out.displayName == "Dai hoc Bach Khoa");
        CHECK(out.searchKey == "dai hoc bach khoa");
        out = StopInfo{"sentinel", "keep", "keep"};
        CHECK(!app.findStop("S012 ", out) && out.stopId == "sentinel");
        CHECK(!app.findStop("s012", out) && out.stopId == "sentinel");
        CHECK(!app.findStop("", out) && out.stopId == "sentinel");
        CHECK(app.removeStop("S012"));
        CHECK(!app.findStop("S012", out) && out.stopId == "sentinel");
    });

    suite.add("invalid records are rejected without changing the catalog", [] {
        BKMove app;
        exampleCatalog(app);
        CHECK(!app.addStop(stop("", "Empty id", "empty id")));
        CHECK(!app.addStop(stop("S100", "", "no name")));
        CHECK(!app.addStop(stop("S101", "No key", "")));
        CHECK(!app.addStop(stop("", "", "")));
        CHECK(app.getStopCount() == 7);
        CHECK(app.findStops("", 0, 100).size() == 7);
        // Rejected duplicates keep the original record byte for byte.
        CHECK(!app.addStop(stop("S001", "Different", "different")));
        StopInfo out;
        CHECK(app.findStop("S001", out) && out.displayName == "Ben Thanh");
        CHECK(out.searchKey == "ben thanh");
        CHECK(app.findStops("different", 0, 5).empty());
        CHECK(app.getStopCount() == 7);
    });

    suite.add("findStops does not mutate the catalog", [] {
        BKMove app;
        exampleCatalog(app);
        const std::vector<StopInfo> first = app.findStops("ben", 0, 3);
        for (int i = 0; i < 20; ++i) app.findStops("ben", i % 3, 2);
        const std::vector<StopInfo> again = app.findStops("ben", 0, 3);
        checkSameRows(again, first);
        CHECK(app.getStopCount() == 7);
        CHECK(app.findStops("", 0, 100).size() == 7);
    });

    suite.add("randomized differential against a sorted reference", [] {
        BKMove app;
        std::vector<StopInfo> model;
        const std::vector<std::string> keys = {"ben thanh", "ben xe", "ben xe cho lon",
                                               "ben xe mien tay", "cho lon", "ha noi",
                                               "hai phong", "hue", "nha trang", "sai gon",
                                               "vung tau", "da lat"};
        unsigned seed = 20261101u;
        int nextId = 0;
        for (int step = 0; step < 900; ++step) {
            const unsigned action = nextRandom(seed) % 10;
            if (action < 5) {
                const std::string& key = keys[nextRandom(seed) % keys.size()];
                const std::string id = "S" + std::to_string(nextId++ % 40);
                const bool added = app.addStop(stop(id, key, key));
                const bool duplicate = std::any_of(model.begin(), model.end(),
                                                   [&](const StopInfo& s) { return s.stopId == id; });
                CHECK(added == !duplicate);
                if (added) model.push_back(stop(id, key, key));
            } else if (action < 7) {
                const std::string id = "S" + std::to_string(nextRandom(seed) % 40);
                const bool removed = app.removeStop(id);
                const auto it = std::find_if(model.begin(), model.end(),
                                             [&](const StopInfo& s) { return s.stopId == id; });
                CHECK(removed == (it != model.end()));
                if (it != model.end()) model.erase(it);
            } else {
                const std::string& key = keys[nextRandom(seed) % keys.size()];
                std::string prefix = key.substr(0, nextRandom(seed) % (key.size() + 1));
                if (nextRandom(seed) % 5 == 0) prefix += "z";
                const int page = static_cast<int>(nextRandom(seed) % 6) - 1;
                const int pageSize = static_cast<int>(nextRandom(seed) % 5);
                checkSameRows(app.findStops(prefix, page, pageSize),
                              referencePage(model, prefix, page, pageSize));
            }
            CHECK(app.getStopCount() == static_cast<int>(model.size()));
        }
        checkSameRows(app.findStops("", 0, 1000), referencePage(model, "", 0, 1000));
        checkSameRows(app.findStops("ben xe", 0, 1000), referencePage(model, "ben xe", 0, 1000));
        checkSameRows(app.findStops("ha", 1, 2), referencePage(model, "ha", 1, 2));
    });

    suite.add("deep page on a 100k catalog costs the same as the first page", [] {
        BKMove app;
        constexpr int N = 100000;
        std::string digits;
        for (int i = 0; i < N; ++i) {
            digits.clear();
            for (int value = i; value > 0; value /= 10) digits.push_back(static_cast<char>('0' + value % 10));
            if (digits.empty()) digits = "0";
            std::reverse(digits.begin(), digits.end());
            while (digits.size() < 6) digits.insert(digits.begin(), '0');
            CHECK(app.addStop(stop("S" + digits, "Stop " + digits, "stop " + digits)));
        }
        CHECK(app.getStopCount() == N);
        const std::vector<StopInfo> deep = app.findStops("stop", 5000, 10);
        CHECK(deep.size() == 10 && deep.front().stopId == "S050000" && deep.back().stopId == "S050009");
        const std::vector<StopInfo> shallow = app.findStops("stop", 0, 10);
        CHECK(shallow.size() == 10 && shallow.front().stopId == "S000000");

        typedef std::chrono::steady_clock clock_type;
        const int rounds = 2000;
        clock_type::time_point begin = clock_type::now();
        for (int i = 0; i < rounds; ++i) CHECK(app.findStops("stop", 0, 10).size() == 10);
        const double shallowMs = std::chrono::duration<double, std::milli>(clock_type::now() - begin).count();

        begin = clock_type::now();
        for (int i = 0; i < rounds; ++i) CHECK(app.findStops("stop", 5000, 10).size() == 10);
        const double deepMs = std::chrono::duration<double, std::milli>(clock_type::now() - begin).count();

        // A rank walk is O(log n) either way; a linear scan would make the deep
        // page ~5000x slower than the first one.
        CHECK(deepMs < 20 * shallowMs + 40.0);
    });

    suite.add("drain and rebuild keeps the catalog consistent", [] {
        BKMove app;
        std::vector<std::string> idsInOrder;
        for (int i = 0; i < 500; ++i) {
            const std::string id = "S" + std::to_string(1000 + i);
            const std::string key = "stop " + std::to_string(i % 7);
            CHECK(app.addStop(stop(id, key, key)));
            idsInOrder.push_back(id);
        }
        CHECK(app.getStopCount() == 500);
        CHECK(app.findStops("stop 3", 0, 1000).size() == 71);
        for (std::size_t i = 0; i < idsInOrder.size(); ++i) {
            if (i % 2 == 0) CHECK(app.removeStop(idsInOrder[i]));
        }
        CHECK(app.getStopCount() == 250);
        CHECK(app.findStops("", 0, 1000).size() == 250);
        for (std::size_t i = 0; i < idsInOrder.size(); ++i) {
            if (i % 2 == 1) CHECK(app.removeStop(idsInOrder[i]));
        }
        CHECK(app.getStopCount() == 0);
        CHECK(app.findStops("", 0, 1000).empty());
        CHECK(app.addStop(stop("S1", "Ben Thanh", "ben thanh")));
        CHECK(app.getStopCount() == 1);
        CHECK(app.findStops("ben", 0, 10).size() == 1);
    });

    return suite.run(argc, argv);
}
