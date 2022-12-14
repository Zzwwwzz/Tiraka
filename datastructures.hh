// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <memory>
#include <list>
#include <queue>
#include <cmath>

// Type for beacon IDs
using BeaconID = std::string;

// Return value for cases where required beacon was not found
BeaconID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for color (RGB)
struct Color
{
    int r = NO_VALUE;
    int g = NO_VALUE;
    int b = NO_VALUE;
};

// Equality and non-equality comparisons for Colors
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1==c2); }

// Return value for cases where color was not found
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost (used only in the second assignment)
using Cost = int;

// Return value for cases where cost is unknown
Cost const NO_COST = NO_VALUE;


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance:         O(1)
    // Short rationale for estimate:    std::vector::size on vakioaikainen operaattori
    int beacon_count();

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    Molemmat vectorin ja mapon clear on lineaarinen rakenteen koon mukaan
    void clear_beacons();

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    Palautusvecotriin majakoiden lis????minen on lineaarinen rakenteen koon mukaan,
    //                                  koska joudutaan k??ym????n l??pi jokainen alkio
    std::vector<BeaconID> all_beacons();

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Rajoittava tekij?? on majakoiden lis??ys unordered_mappiin
    //                                  sek?? unordered_map::find. Muut operaatiot vakioaikaisia.
    bool add_beacon(BeaconID id, std::string const& name, Coord xy, Color color);

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Rajoittava tekij?? unordered_map::find, muut vakioaikaisisa
    std::string get_name(BeaconID id);

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Rajoittava tekij?? unordered_map::find, muut vakioaikaisisa
    Coord get_coordinates(BeaconID id);

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Rajoittava tekij?? unordered_map::find, muut vakioaikaisisa
    Color get_color(BeaconID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:         O(n log n)
    // Short rationale for estimate:    Vectorin sort on rajoittava. Vectoriin lis??ys lineaarinen ja
    //                                  muut vakioaikaisia. Jos tietorakenteeseen ei ole lis??tty uusia
    //                                  majakoita viime j??rjest??misen j??lkeen, O(1). Olisi my??s
    //                                  mahdollsita toteuttaa mergen avulla O(n).
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance:         O(n log n)
    // Short rationale for estimate:    Vectorin sort on rajoittava. Vectoriin lis??ys lineaarinen ja
    //                                  muut vakioaikaisia. Jos tietorakenteeseen ei ole lis??tty uusia
    //                                  majakoita viime j??rjest??misen j??lkeen, O(1). Olisi my??s
    //                                  mahdollsita toteuttaa mergen avulla O(n).
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance:         O(1)
    // Short rationale for estimate:    Funktiossa palautetaan ennalta tiedetty arvo
    BeaconID min_brightness();

    // Estimate of performance:         O(1)
    // Short rationale for estimate:    Funktiossa palautetaan ennalta tiedetty arvo
    BeaconID max_brightness();

    // Estimate of performance:         O(n log n)
    // Short rationale for estimate:    Lopussa joudutaan j??rjest??m????n palautettava vectori, vaikkakin
    //                                  j??rjestett??vien alkioiden m????r?? on harvoin suuri. Muuten lineaarinen.
    std::vector<BeaconID> find_beacons(std::string const& name);

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Majakan etsiminen tietorakenteesta rajoittaa muuten vakioaikaista
    //                                  funktiota
    bool change_beacon_name(BeaconID id, std::string const& newname);

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Majakan etsiminen tietorakenteesta rajoittaa. Muut vakioaikaista
    //                                  funktiota
    bool change_beacon_color(BeaconID id, Color newcolor);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:         Keskim????rin O(1), Huonoimmassa tapauksessa O(n)
    // Short rationale for estimate:    Majakan etsiminen tietorakenteesta rajoittaa muuten vakioaikaista
    //                                  funktiota
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance:         O(n log n)
    // Short rationale for estimate:    Palautettava vector joudutaan j??rjest??m????n uudelleen. J??rjestett??vien
    //                                  alkioiden m????r?? on harvoin kovin suuri. Muuuten O(n).
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    Vectoriin alkioiden lis??ys ID:lt?? juureen on rajoittava tekij??
    std::vector<BeaconID> path_outbeam(BeaconID id);

    // Non-compulsory operations

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    vecotrin alkion haku ja poisto O(n), uuden kirkkaimman/himmeimm??n
    //                                  etsiminen O(n), unordered_map keskim????rin O(1), huonoimmassa tapauksessa O(n)
    bool remove_beacon(BeaconID id);

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    Lehtimajakan etsiminen ja reitin lis????minen vectoriin on molemmat
    //                                  lineaarisia alkioiden m????r??n mukaan.
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance:         O(n)
    // Short rationale for estimate:    Jokaisen majakan lapsen l??pik??ynti rajoittaa funktion nopeuteen O(n).
    Color total_color(BeaconID id);

    // Phase 2 operations

    // Estimate of performance:         O(V log V)
    // Short rationale for estimate:    Funktiossa palautetaan joko tiedossa oleva j??rjestyksess?? oleva vector O(1)
    //                                  tai k??yd????n jokainen kaaren p????ss?? oleva solmu l??pi ja j??rjestet????n ne.
    std::vector<Coord> all_xpoints();

    // Estimate of performance:         O(log V)
    // Short rationale for estimate:    Solmujen etsiminen tietorakenteesta rajoittava tekij??.
    //                                  Muuten funktio on keskim????rin vakioaikainen.
    bool add_fibre(Coord xpoint1, Coord xpoint2, Cost cost);

    // Estimate of performance:         O(V log V)
    // Short rationale for estimate:    Rajoittavana tekij??n?? on kaikkien naapureiden j??rjest??minen suuruusj??rjestykseen.
    //                                  Alkiom????r??t tosin pysyv??t melko pienin??, joten en koe tarpeelliseksi lis??t?? erillist??
    //                                  lippua ja tietorakennetta pit????kseni dataa muistissa.
    //                                  Muutenkomento on O(V)
    std::vector<std::pair<Coord, Cost>> get_fibres_from(Coord xpoint);

    // Estimate of performance:         O(E log E)
    // Short rationale for estimate:    Jos j??rjestys on jo tiedossa, O(1). Muuten rajoittavana tekij??n?? ei-j??rjestettyjen
    //                                  valokuitujen j??rjest??minen. Muuten funktio on lineaarinen.
    std::vector<std::pair<Coord, Coord>> all_fibres();

    // Estimate of performance:         O(E)
    // Short rationale for estimate:    Poistaessa joudutaan k??ym????n kaikki valokuidut l??pi.
    bool remove_fibre(Coord xpoint1, Coord xpoint2);

    // Estimate of performance:         O(E)/O(V)
    // Short rationale for estimate:    Molempien s??ili??iden clear() on lineaarinen s??ili??n kokoon n??hden.
    void clear_fibres();

    // Estimate of performance:         O(V+E)
    // Short rationale for estimate:    While -loop toimii vain solmujen m????r??n verran
    //                                  ja for -loop vain kaarien m????r??n verran.
    std::vector<std::pair<Coord, Cost>> route_any(Coord fromxpoint, Coord toxpoint);

    // Non-compulsory operations

    // Estimate of performance:         O(V+E)
    // Short rationale for estimate:    While -loop toimii vain solmujen m????r??n verran
    //                                  ja for -loop vain kaarien m????r??n verran.
    std::vector<std::pair<Coord, Cost>> route_least_xpoints(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:         O(V+E)
    // Short rationale for estimate:    While -loop toimii vain solmujen m????r??n verran
    //                                  ja for -loop vain kaarien m????r??n verran.
    std::vector<std::pair<Coord, Cost>> route_fastest(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:         O(V+E)
    // Short rationale for estimate:    While -loop toimii vain solmujen m????r??n verran
    //                                  ja for -loop vain kaarien m????r??n verran.
    std::vector<Coord> route_fibre_cycle(Coord startxpoint);

    // Estimate of performance:         ~O(E log E)
    // Short rationale for estimate:    Rajoittavana tekij??n?? mahdollisesti vaadittu
    //                                  kaarien j??rjest??minen. Muuten O(E+V).
    Cost trim_fibre_network();

private:
    // Add stuff needed for your class implementation here
    struct Majakka
    {
        // prg1
        std::string id;
        std::string nimi;
        Coord sijainti;
        Color vari;
        int kirkkaus;

        std::shared_ptr<Majakka> meneva = nullptr;
        std::vector<std::shared_ptr<Majakka>> tulevat{};     
    };

    enum Solmustatus {White, Gray, Black};
    struct Solmu
    {
      int indeksi;
      Coord sijainti;

      Solmustatus vari;
      std::shared_ptr<Solmu> edellinen;
      int etaisyys;

      std::unordered_map<Coord, int, CoordHash> naapurit;
    };

    struct Kuitu
    {
      std::shared_ptr<Solmu> solmu1;
      std::shared_ptr<Solmu> solmu2;
      int hinta;
    };


    
    // prg2   

    std::vector<std::shared_ptr<Kuitu>> kuidut{};
    std::vector<std::pair<Coord, Coord>> lisatyt_kuidut = {};
    std::unordered_map<Coord, std::shared_ptr<Solmu>, CoordHash> solmu_sijainti{};

    bool is_sorted_xpoints = false;
    std::vector<Coord> sorted_xpoints = {};

    bool is_sorted_fibres = false;
    std::vector<std::pair<Coord, Coord>> sorted_fibres = {};

    bool is_sorted_weight = false;
    std::vector<std::shared_ptr<Kuitu>> sorted_weight = {};

    bool is_spanning_tree = false;
    int spanning_tree_cost = 0;

    bool poistettu_kuituja = false;


    void paivita_matriisi(); // O(V)
    void alusta(); // O(V)

   void tulosta_reitti(std::vector<std::pair<Coord, Cost>> &reitti, int &cost,
                       Coord fromxpoint, Coord toxpoint); // O(V)

   void relax_a(Coord solmu1, Coord solmu2, Coord loppupiste); // O(1)

   void tulosta_sykli(std::vector<Coord> &reitti,
                       Coord fromxpoint, Coord toxpoint); // O(V)

   int etsi_vanhempi(int &solmu, std::vector<int> &disjoint_set); // O(V)
   bool isEdge(Coord solmu1, Coord solmu2); // O(log V)
   int kaari_hinta(Coord solmu1, Coord solmu2); // O(1)

    
    // prg1
    std::unordered_map<BeaconID, std::shared_ptr<Majakka>> majakat;
    std::vector<std::shared_ptr<Majakka>> majakat_vektori;

    bool is_sorted_alpha = false;
    bool is_sorted_bright = false;
    std::shared_ptr<std::vector<BeaconID>> sorted_alpha_ptr;
    std::shared_ptr<std::vector<BeaconID>> sorted_bright_ptr;

    BeaconID min_kirkkausid = NO_ID; // S??ilytt???? himmeimm??n ID:n
    int min_kirkkaus = 8000; // Ei voida ylitt???? oikealla sy??tteell??
    BeaconID max_kirkkausid = NO_ID; // S??ilytt???? kirkkaimman ID:n
    int max_kirkkaus = -1; // Ei voida alittaa oikealla sy??tteell??

    int laske_kirkkaus(Color color); // O(1)

    void laske_syvyys(BeaconID id, int syvyys, int& maxsyvyys,
                     BeaconID& lehti_id); // O(n)

    Color laske_vari(BeaconID id); // O(n)
};

#endif // DATASTRUCTURES_HH
