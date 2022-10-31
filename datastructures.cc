// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{

}

Datastructures::~Datastructures()
{

}

int Datastructures::beacon_count()
{
    // Palauttaa majakoiden lukumäärän (int)
    return majakat_vektori.size();
}

void Datastructures::clear_beacons()
{
    // Poistaa kaikista tietorakenteista kaikki majakat
    majakat.clear();
    majakat_vektori.clear();
    is_sorted_alpha = false;
    is_sorted_bright = false;
    min_kirkkausid = NO_ID;
    max_kirkkausid = NO_ID;
    min_kirkkaus = 8000;
    max_kirkkaus = -1;

}

std::vector<BeaconID> Datastructures::all_beacons()
{
    // Palauttaa jokaisen majakan ID:n vectorina
    std::vector<std::string> majakkavektori;
    majakkavektori.reserve(majakat.size());

    // Käydään läpi jokainen majakka ja lisätään ne palautettavaan vectoriin
    for (auto const& majakka : majakat_vektori){
        majakkavektori.push_back(majakka->id);
    }

    return majakkavektori;
}

bool Datastructures::add_beacon(BeaconID id, const std::string& name, Coord xy, Color color)
{
    // Lisää teitorakenteisiin uuden majakan

    // Jos majakka löytyy jo tietorakenteesta, ei tehdä mitään
    if (majakat.find(id) == majakat.end()){

        is_sorted_alpha = false;
        is_sorted_bright = false;

        // Luodaan majakkaolio ja luodaan siihen osoittava pointteri
        Majakka beacon = {id, name, xy, color, laske_kirkkaus(color)};
        std::shared_ptr<Majakka> majakkaosoitin(std::make_shared<Majakka>(beacon));

        // Tarkastetaan, että onko majakan kirkkaus suurin tai pienin teitorakenteessa
        if (beacon.kirkkaus < min_kirkkaus){
            min_kirkkausid = id;
            min_kirkkaus = beacon.kirkkaus;
        }
        if (beacon.kirkkaus > max_kirkkaus){
            max_kirkkausid = id;
            max_kirkkaus = beacon.kirkkaus;
        }

        // Lisätään majakka teitorakenteiiin
        majakat.insert({id, majakkaosoitin});
        majakat_vektori.push_back(majakkaosoitin);
        return true;
    } else{
        return false;
    }
}

std::string Datastructures::get_name(BeaconID id)
{
    // Tutkitaan, että löytyykö majakka teitorakenteesta
    if (majakat.find(id) != majakat.end()){
        return majakat.at(id)->nimi;
    } else
        return NO_NAME;
}

Coord Datastructures::get_coordinates(BeaconID id)
{
    // Tutkitaan, että löytyykö majakka teitorakenteesta
    if (majakat.find(id) != majakat.end()){
        return majakat.at(id)->sijainti;
    } else{
        return NO_COORD;
    }
}

Color Datastructures::get_color(BeaconID id)
{
    // Tutkitaan, että löytyykö majakka teitorakenteesta
    if (majakat.find(id) != majakat.end()){
        return majakat.at(id)->vari;
    } else{
        return NO_COLOR;
    }
}

std::vector<BeaconID> Datastructures::beacons_alphabetically()
{
    // Palauttaa majakat aakkosjärjestyksessä

    // Tarkastetaan, onko vektori jo järjestyksessä
    if(!is_sorted_alpha){

        std::vector<BeaconID> alphamajakkavektori;
        alphamajakkavektori.reserve(majakat_vektori.size());

        // Järjestetään majakat niiden nimen mukaisesti
        std::sort(majakat_vektori.begin(), majakat_vektori.end(),
                  [&](const std::shared_ptr<Majakka>& lhs, const std::shared_ptr<Majakka>& rhs)
        {return lhs->nimi < rhs->nimi;});

        // Lisätään majakat palautettavaan vectoriin
        for (auto const& majakka : majakat_vektori){
            alphamajakkavektori.push_back(majakka->id);
        }

        // Tallennetaan tieto järjestetystä vectorista ja tallennetaan se osoittimeen
        is_sorted_alpha = true;
        sorted_alpha_ptr = std::make_shared<std::vector<BeaconID>>(alphamajakkavektori);

        return alphamajakkavektori;
    } else {
        return *sorted_alpha_ptr;
    }
}

std::vector<BeaconID> Datastructures::beacons_brightness_increasing()
{
    // Palauttaa majakat kirkkausjärjestyksessä

    // Tarkastetaan, onko vektori jo järjestyksessä
    if(!is_sorted_bright){
        std::vector<BeaconID> brightmajakkavektori;
        brightmajakkavektori.reserve(majakat_vektori.size());

        // Järjestetään majakat niiden kirkkauden mukaisesti
        std::sort(majakat_vektori.begin(), majakat_vektori.end(),
                  [&](const std::shared_ptr<Majakka>& lhs, const std::shared_ptr<Majakka>& rhs)
        {return (lhs->kirkkaus) < (rhs->kirkkaus);} );

        // Lisätään majakat palautettavaan vectoriin
        for (auto const& majakka : majakat_vektori){
            brightmajakkavektori.push_back(majakka->id);
        }

        // Tallennetaan tieto järjestetystä vectorista ja tallennetaan se osoittimeen
        is_sorted_bright = true;
        sorted_bright_ptr = std::make_shared<std::vector<BeaconID>>(brightmajakkavektori);

        return brightmajakkavektori;
    } else {
        return *sorted_bright_ptr;
    }
}

BeaconID Datastructures::min_brightness()
{
    // Tarkastetaan onko majakoita jäljellä
    if (majakat_vektori.size() != 0){
        return min_kirkkausid;
    } else {
        return NO_ID;
    }
}

BeaconID Datastructures::max_brightness()
{
    // Tarkastetaan onko majakoita jäljellä
    if (majakat_vektori.size() != 0){
        return max_kirkkausid;
    } else {
        return NO_ID;
    }
}

std::vector<BeaconID> Datastructures::find_beacons(std::string const& name)
{
    // Etsitään majakat nimen perusteella
    std::vector<BeaconID> etsityt_majakat;
    etsityt_majakat.reserve(majakat.size());

    // Etsitään teitokannasta annetun nimen mukaiset majakat
    for (auto const& majakka : majakat_vektori){
        if (majakka->nimi == name){
            etsityt_majakat.push_back(majakka->id);
        }
    }

    // Palautetaan majakat ID:n mukaisessa aakkosjärjestyksessä
    if (!etsityt_majakat.empty()){
        std::sort(etsityt_majakat.begin(), etsityt_majakat.end());
        return etsityt_majakat;
    } else{
        return {};
    }
}

bool Datastructures::change_beacon_name(BeaconID id, const std::string& newname)
{
    // Vaihtaa majakan nimen
    if (majakat.find(id) == majakat.end()){
        return false;
    } else{
        majakat.at(id)->nimi = newname;
        return true;
    }
}

bool Datastructures::change_beacon_color(BeaconID id, Color newcolor)
{
    // Vaihtaa majakat värin
    if (majakat.find(id) == majakat.end()){
        return false;
    } else{
        is_sorted_bright = false;

        // Vaihdetaan väri ja laksetaan uusi majakan kirkkaus
        majakat.at(id)->vari = newcolor;
        majakat.at(id)->kirkkaus = laske_kirkkaus(newcolor);

        // Tutkitaan onko majakka kirkkain tai himmein
        if (majakat.at(id)->kirkkaus < min_kirkkaus){
            min_kirkkausid = id;
            min_kirkkaus = majakat.at(id)->kirkkaus;
        }
        if (majakat.at(id)->kirkkaus > max_kirkkaus){
            max_kirkkausid = id;
            max_kirkkaus = majakat.at(id)->kirkkaus;
        }
        return true;
    }
}

bool Datastructures::add_lightbeam(BeaconID sourceid, BeaconID targetid)
{
    // Lisää valonsäteen majakasta toiseen

    // Tutkitaan löytyykö molemmat majakat teitorakenteesta
    if ((majakat.find(sourceid) == majakat.end()) ||
            (majakat.find(targetid) == majakat.end())){
        return false;
    }
    // Jos majakka lähettää valoa jo toiselle majakalle, ei tehdä mitään
    if (majakat.at(sourceid)->meneva != nullptr){
        return false;
    } else {
        // Lisätään tieto valonsäteestä pointtereina lähettävään majakkaan
        // sekä vastaanottavaan majakkaan
        majakat.at(sourceid)->meneva = majakat.at(targetid);
        majakat.at(targetid)->tulevat.push_back(majakat.at(sourceid));
        return true;
    }
}

std::vector<BeaconID> Datastructures::get_lightsources(BeaconID id)
{
    // Palauttaa valonlähteet
    // Tutkitaan, että onko majakka olemassa ja onko se tehjä
    if (majakat.find(id) == majakat.end()){
        return {{NO_ID}};
    } else if (majakat.at(id)->tulevat.size() == 0) {
        return {};

    } else {
        std::vector<BeaconID> palautettavat_majakat;
        palautettavat_majakat.reserve(majakat.at(id)->tulevat.size());

        // Lisätään palautettavaan vektoriin valoa lähettävien majakoiden ID:t
        for (auto const& majakka : majakat.at(id)->tulevat){
            palautettavat_majakat.push_back(majakka->id);
        }

        // Ja järjestetänn ne aakkosjärjestykseen
        std::sort(palautettavat_majakat.begin(), palautettavat_majakat.end());
        return palautettavat_majakat;
    }
}

std::vector<BeaconID> Datastructures::path_outbeam(BeaconID id)
{
    // Palauuttaa majakat, joille majakka lähettää suorasti/epäsuorasti valoa
    if (majakat.find(id) == majakat.end()){
        return {{NO_ID}};
    }
    std::vector<BeaconID> palautettavat_majakat;
    palautettavat_majakat.reserve(majakat_vektori.size());

    std::shared_ptr<Majakka> to_be_printed = majakat.at(id);
    // Lisätään kaikki majakat palautettavaan vectoriin kutsutulta majakalta
    // juureen
    while( to_be_printed != nullptr ) {
       palautettavat_majakat.push_back(to_be_printed->id);
       to_be_printed = to_be_printed->meneva;
        }
    return palautettavat_majakat;
}

bool Datastructures::remove_beacon(BeaconID id)
{
    // Poistaa majakan tietorakenteesta
    if (majakat.find(id) == majakat.end()){
        return false;
    }

    // Jos poistettava majakka lähettää valoa
    if (majakat.at(id)->meneva != nullptr){

        // Selvittää majakan indexin
        int running_nmbr = 0;

        // Etsitään poistettava majakka vectorista, joka on majakassa, johon poistettava majakka lähettää
        // valoa
        for (auto majakkapointteri : majakat.at(id)->meneva->tulevat){
            if (majakkapointteri->id == id){
                // Poistetaan koko alkio
                majakat.at(id)->meneva->tulevat.erase(majakat.at(id)->meneva->tulevat.begin()+running_nmbr);
                break;
            }
            running_nmbr += 1;
        }

    }

    // Poistetaan majakalle tulevat valonsäteet
    if (majakat.at(id)->tulevat.size() != 0){
        for (auto majakkapointteri : majakat.at(id)->tulevat){
            majakkapointteri->meneva = nullptr;
        }
    }

    // Pitää yllä halutun arvon indeksiä
    int running_nmbr = 0;
    // Poistetaan majakka vector -tietorakenteesta
    for (auto majakkapointteri : majakat_vektori){
            if (majakkapointteri->id == id){
                majakat_vektori.erase(majakat_vektori.begin()+running_nmbr);
                break;
            }
        running_nmbr += 1;
    }

    // Positetaan majakka map -tietorakenteesta
    majakat.erase(id);

    // Tutkitaan onko poistettu majakka tietorakenteen kirkkain tai himmein
    // Jos on, haetaan tietorakenteen toiseksi kirkkain majakka
    if (id == max_kirkkausid){
        max_kirkkaus = -1;
        for (auto majakka : majakat_vektori){
            if (majakka->kirkkaus > max_kirkkaus){
                max_kirkkaus = majakka->kirkkaus;
                max_kirkkausid = majakka->id;
            }
        }

    }
    if (id == min_kirkkausid){
        min_kirkkaus = 8000;
        for (auto majakka : majakat_vektori){
            if (majakka->kirkkaus < min_kirkkaus){
                min_kirkkaus = majakka->kirkkaus;
                min_kirkkausid = majakka->id;
            }
        }
    }

    // Majakat ovat järjestyksessä, mutta alkioiden poisto valitussa tietorakenteessa on hankalaa
    is_sorted_alpha = false;
    is_sorted_bright = false;
    return true;
}

std::vector<BeaconID> Datastructures::path_inbeam_longest(BeaconID id)
{
    // Palauttaa pisimmän mahdollisen ketjun valonsäteitä,
    // jotka päätyvät annettuun majakkaan
    if (majakat.find(id) == majakat.end()){
        return {{NO_ID}};

    } else {
        // lehti_id on sen majakan ID, joka on kauimpana majakasta
        BeaconID lehti_id;
        int syvyys = 0;

        // Haetaan rekursiivisesti majakasta kauimpana oleva lehtialkio
        laske_syvyys(id, 1, syvyys, lehti_id);
        std::vector<BeaconID> reitti;
        reitti.reserve(majakat_vektori.size());

        std::shared_ptr<Majakka> to_be_printed = majakat.at(lehti_id);

        // Lisätään kaikki majakat palautettavaan vectoriin lehtimajakalta
        // kutsuttuun majakkaan
        while( to_be_printed->id != id ) {
           reitti.push_back(to_be_printed->id);
           to_be_printed = to_be_printed->meneva;
            }
        // Lisätään lopuksi vielä alunperin kutsuttu majakka
        reitti.push_back(id);
        return reitti;
    }
}

Color Datastructures::total_color(BeaconID id)
{
    // Laskee majakan värin
    if (majakat.find(id) == majakat.end()){
        return NO_COLOR;
    } else{
        Color vari = laske_vari(id);
        return vari;
    }
}

int Datastructures::laske_kirkkaus(Color color){
    // Laskee majakan kirkkauden
    return color.r*3+color.g*6+color.b;
}

void Datastructures::laske_syvyys( const BeaconID id, int syvyys, int& maxsyvyys,
                                 BeaconID& lehti_id){
    // Palauttaa kutsuttua majakkaa kauimpana olevan majakan

    // Jos majakalle ei mikään majakka lähetä valoa, on se puurakenteen lehti
    if (majakat.at(id)->tulevat.size() == 0) {

        // Jos majakka on puurakenteen syvin, tallennetaan syvyys sekä majakan ID
        if (maxsyvyys < syvyys){
            maxsyvyys = syvyys;
            lehti_id = id;
        }
        // Palataan puurakennetta askel taaksepäin
        return;
    }
    // Käydään läpi jokainen kutsutulle ID:lle valoa lähettävä majakka
    for (auto majakkapointteri : majakat.at(id)->tulevat){
        laske_syvyys(majakkapointteri->id, syvyys+1, maxsyvyys, lehti_id);

    }
}

Color Datastructures::laske_vari(BeaconID id){

    // Jos majakalle ei mikään majakka lähetä valoa, on se puurakenteen lehti
    if (majakat.at(id)->tulevat.size() == 0) {
        // Palautetaan jokaiselta lehdeltä sen väri
        return Color {majakat.at(id)->vari.r, majakat.at(id)->vari.g, majakat.at(id)->vari.b};
    }

    // Otetaan ylös kutsun majakan värikomponentit
    int r = majakat.at(id)->vari.r;
    int g = majakat.at(id)->vari.g;
    int b = majakat.at(id)->vari.b;

    // Lasketaan rekursiivisesti jokaisen majakalle tulevan valon väri
    for (auto majakkapointteri : majakat.at(id)->tulevat){
        Color newcolor = laske_vari(majakkapointteri->id);
        r += newcolor.r;
        g += newcolor.g;
        b += newcolor.b;
    }

    // Lasketaan värien keskiarvo
    int majakka_lkm = majakat.at(id)->tulevat.size()+1;
    return Color{r/majakka_lkm, g/majakka_lkm, b/majakka_lkm};
}


std::vector<Coord> Datastructures::all_xpoints()
{
    /**
    @todo  merge?
    **/

    // Tutkitaan onko ohjelmassa tallessa jo tarvittava tieto ja palautetaan se
    if (is_sorted_xpoints){
        return sorted_xpoints;
    } else {     

        // Alustetaan palautettava vector
        std::vector<Coord> yhdistetyt_solmut;
        yhdistetyt_solmut.reserve(solmu_sijainti.size());

        // Käydään läpi jokainen solmu
        for (auto const& kuitu : kuidut){ // O(V)

            // Tutkitaan molemmat solmut siitä, että onko solmu jo löydetty
            if (kuitu->solmu1->vari == White){
                // Jos ei, lisätään ja merkitään löydetyksi
                yhdistetyt_solmut.push_back(kuitu->solmu1->sijainti);
                kuitu->solmu1->vari = Black;
            }
            if (kuitu->solmu2->vari == White){
                yhdistetyt_solmut.push_back(kuitu->solmu2->sijainti);
                kuitu->solmu2->vari = Black;
            }
        }

        // Alustetaan solmun status takaisin normaalitilaan
        for (auto const& kuitu : kuidut){ // O(E)
            kuitu->solmu1->vari = White;
            kuitu->solmu2->vari = White;
        }

        // Jätjestetään solmut haluttuun järjestykseen
        std::sort(yhdistetyt_solmut.begin(), yhdistetyt_solmut.end()); // O(V log V)

        // Tallennetaan tieto järjestetyistä solmuista
        sorted_xpoints = yhdistetyt_solmut;

        // Ja merkitään muistiin tieto olemassa olevasta järjestyksestä
        is_sorted_xpoints = true;
        return yhdistetyt_solmut;
    }
}

bool Datastructures::add_fibre(Coord xpoint1, Coord xpoint2, Cost cost)
{
    // Kuitu ei voi osoittaa itseensä
    if (xpoint1 == xpoint2){
        return false;
    } else {

        // Etsitään molempien päätepisteiden indeksi
        int indeksi1;
        int indeksi2;

        // Jos päätepistettä ei löydy tietorakenteesta
        if (solmu_sijainti.find(xpoint1) != solmu_sijainti.end()){
            // Jos päätepiste löytyy jo, etsitään sen indeksi
           indeksi1 = solmu_sijainti[(xpoint1)]->indeksi;
        } else {
            // Etsitään seuraava indeksi
            indeksi1 = solmu_sijainti.size();

            // Luodaan Solmu -olio sekä osoitin luotuun olioon
            // indeksi - sijainti - väri - osoitin edelliseen - paino - lähtevät solmut
            Solmu solmu1 = {indeksi1, xpoint1, White, nullptr, 65000, {}};
            std::shared_ptr<Solmu> solmuosoitin(std::make_shared<Solmu>(solmu1));

            // Lisätään osoittimet tietorakenteeseen
            solmu_sijainti.insert(std::make_pair(xpoint1, solmuosoitin));

        }
        // Ja sama toiselle solmulle
        if (solmu_sijainti.find(xpoint2) != solmu_sijainti.end()){
            indeksi2 = solmu_sijainti[(xpoint2)]->indeksi;

        } else {
            indeksi2 = solmu_sijainti.size();

            Solmu solmu2 = {indeksi2, xpoint2, White, nullptr, 65000, {}};
            std::shared_ptr<Solmu> solmuosoitin(std::make_shared<Solmu>(solmu2));

            solmu_sijainti.insert(std::make_pair(xpoint2, solmuosoitin));
         }

        // Jos päätepisteiden välillä ei ole kuitua
        if (!isEdge(xpoint1, xpoint2)){ //

            // Lisätään tieto kuidusta molempiin solmu -tietueihin
            solmu_sijainti[xpoint1]->naapurit.insert(std::make_pair(xpoint2, cost));
            solmu_sijainti[xpoint2]->naapurit.insert(std::make_pair(xpoint1, cost));

            // Tutkitaan kumpi sijainti on pienempi
            if (xpoint2 < xpoint1){

                //Luodaan Kuitu -olio ja osoitin siihen
                Kuitu kuitu = {solmu_sijainti[xpoint2], solmu_sijainti[xpoint1], cost};
                std::shared_ptr<Kuitu> kuituosoitin(std::make_shared<Kuitu>(kuitu));

                // Lisätään kuitu tietorakenteeseen
                kuidut.push_back(kuituosoitin);
                lisatyt_kuidut.push_back(std::make_pair(xpoint2, xpoint1));

            } else {
                Kuitu kuitu = {solmu_sijainti[xpoint1], solmu_sijainti[xpoint2], cost};
                std::shared_ptr<Kuitu> kuituosoitin(std::make_shared<Kuitu>(kuitu));

                kuidut.push_back(kuituosoitin);
                lisatyt_kuidut.push_back(std::make_pair(xpoint1, xpoint2));
            }

            // Merkitään, että aikaisemmat järjestykset eivät enää pidä paikkansa
            is_sorted_fibres = false;
            is_sorted_xpoints = false;
            is_sorted_weight = false;
            is_spanning_tree = false;

            return true;
        } else {

            // Jos kuitu löytyy jo, ei tehdä mitään.
            return false;
        }

    }
}

std::vector<std::pair<Coord, Cost> > Datastructures::get_fibres_from(Coord xpoint)
{
    // Tutkitaan onko solmu jo tietorakenteessa
    if (solmu_sijainti.find(xpoint) != solmu_sijainti.end()){

        // Jos on, alustetaan palautettava vector
        std::vector<std::pair<Coord, Cost>> lahtevat_kuidut;
        lahtevat_kuidut.reserve(solmu_sijainti[(xpoint)]->naapurit.size());

        // Lisätään vectoriin kaikki kutsutun solmun naapurit
        for (auto const& naapuri : solmu_sijainti[(xpoint)]->naapurit){
            lahtevat_kuidut.push_back(std::make_pair(naapuri.first, naapuri.second));
        }

        // Järjestetään naapurit suuruusjärjestykseen ja palautetaan vecotr
        std::sort(lahtevat_kuidut.begin(), lahtevat_kuidut.end());
        return lahtevat_kuidut;

    } else {
        // Jos solmua ei löydy, palautetaan tyhjä vector
        return {};
    }
}

std::vector<std::pair<Coord, Coord> > Datastructures::all_fibres()
{
    // Tutkitaan onko meillä tiedossa jo kaikki valokuidut
    if (is_sorted_fibres){
        // Jos on, palautetaan ne
        return sorted_fibres;
    } else {
        // Tutkitaan onko ohjelman ajon aikana poistettu valokuituja tietämättä kummassa
        // säiliössä se on
        if (poistettu_kuituja){
            // Jos on, tyhjennetään säiliöt
            sorted_fibres.clear();
            lisatyt_kuidut.clear();
            lisatyt_kuidut.reserve(kuidut.size());

            // Ja lisätään siihen kaikki tiedossa olevat kaaret.
            for (auto kuitu : kuidut){
                lisatyt_kuidut.push_back(std::make_pair(kuitu->solmu1->sijainti, kuitu->solmu2->sijainti));
            }
            poistettu_kuituja = false;
        }

        // Alustetaan söiliö, johon lisätään kaikki valokuidut
        std::vector<std::pair<Coord, Coord> > kaikki_kuidut;
        kaikki_kuidut.reserve(kuidut.size());

        // Järjestetään ei-järjestetyt valokuidut
        std::sort(lisatyt_kuidut.begin(), lisatyt_kuidut.end());
        // Ja käytetään mergesort kahteen järjestettyyn vectoriin.
        std::merge(sorted_fibres.begin(), sorted_fibres.end(),
                   lisatyt_kuidut.begin(), lisatyt_kuidut.end(),  std::back_inserter(kaikki_kuidut));

        // Otetaan muistiin järjestetyt valokuidut
        sorted_fibres = kaikki_kuidut;
        // Nollataan ei-järjestettyjen valokuitujen määrä
        lisatyt_kuidut.clear();
        // Ja merkitään tieto järjestetyistä valokuiduista.
        is_sorted_fibres = true;
        return kaikki_kuidut;
    }
}

bool Datastructures::remove_fibre(Coord xpoint1, Coord xpoint2)
{
    // Tutkitaan löytyykö molemmat solmut tietorakenteesta.
    if (solmu_sijainti.find(xpoint1) != solmu_sijainti.end() &&
            solmu_sijainti.find(xpoint2) != solmu_sijainti.end()){
    } else {
        return false;
    }
    // Tutkitaan onko solmujen välillä valokuitu
    if (isEdge(xpoint1, xpoint2)){ // O(log V)
        int running_nmbr = 0;

        // Tutkitaan kumpi sijainneista on pienempi, jotta voidana yhtenäöistämään data
        if (xpoint2 < xpoint1){
            // jos xpoint2 on pienempi, vaihdetaan solmut päittään
            Coord tmp = xpoint1;
            xpoint1 = xpoint2;
            xpoint2 = tmp;
        }

        // Etsitään kuidut -säiliöstä poistettava valokuitu
        for (auto& kuitu : kuidut){
            if ((kuitu->solmu1->sijainti == xpoint1 &&
                    kuitu->solmu2->sijainti == xpoint2 )){

                kuidut.erase(kuidut.begin()+running_nmbr);
                break;
            }
            running_nmbr += 1;
        }
        // is_deleted merkkaa sen, löytyikö poistettava valokuitu ei-järjestetyistä valokuiduista.
        // Jos löytyy, ei tarvitse tutkia enää järjestettyjä valokuituja
        bool is_deleted = false;
        running_nmbr = 0;
        // Tutkitaan läpi kaikki ei-järjestetyt valokuidut
        for (auto kuitu : lisatyt_kuidut){
            // Jos valokuitu löytyy
                if (kuitu.first == xpoint1 && kuitu.second == xpoint2){
                    // poistetaan se ja merkitään valokuitu poistetuksi
                   lisatyt_kuidut.erase(lisatyt_kuidut.begin()+running_nmbr);
                   is_deleted = true;
                    break;
                }
            running_nmbr += 1;
        }
        running_nmbr = 0;
        // Jos valokuitu ei löytynyt ei-järjestetyistä valokuiduista, etsitään se järjestetyistä
        // valokuiduista.
        if (!is_deleted){          
            // Käydään läpi kaikki järjesteyt valokuidut
            for (auto kuitu : sorted_fibres){
                // Jos valokuitu löytyy
                if (kuitu.first == xpoint1 && kuitu.second == xpoint2){
                    // Poistetaan se tietorakenteesta
                    sorted_fibres.erase(sorted_fibres.begin()+running_nmbr);
                    break;
                }
                running_nmbr += 1;
            }
        }

        // Poistetaan molempien solmujen tietueesta tieto ko. solmujen yhteydestä
        if (solmu_sijainti[(xpoint1)]->naapurit.find(xpoint2) != solmu_sijainti[(xpoint1)]->naapurit.end()){
           solmu_sijainti[(xpoint1)]->naapurit.erase(xpoint2);
           solmu_sijainti[(xpoint2)]->naapurit.erase(xpoint1);
        }

        // merkitään liput epätosiksi
        is_sorted_fibres = false;
        is_spanning_tree = false;
        is_sorted_xpoints = false;
        is_sorted_weight = false;


        return true;
    } else{
        return false;
    }
}

void Datastructures::clear_fibres()
{
    // Tyhjennetään kaikki tieotrakenteet
    solmu_sijainti.clear();
    kuidut.clear();
    sorted_fibres.clear();
    lisatyt_kuidut.clear();

    // Ja alustetaan liput
    is_sorted_xpoints = false;
    is_sorted_fibres = false;
    is_spanning_tree = false;
    is_sorted_weight = false;
}

std::vector<std::pair<Coord, Cost> > Datastructures::route_any(Coord fromxpoint, Coord toxpoint)
{
    // toteutetaan DFS-algoritmilla

    // Tutkitaan löytyykö solmu tietorakenteesta
    if ((solmu_sijainti.find(fromxpoint) != solmu_sijainti.end()) &&
            (solmu_sijainti.find(toxpoint) != solmu_sijainti.end())){

    // Käytetään TODO-listana list säiliötä
    std::list<Coord> stack;
    // Lisätään aloituspiste listalle
    stack.push_back(fromxpoint);

    // Jatketaan iterointia niin kauan, kun säiliö ei ole tyhjä
    while(!stack.empty()){ // O(V)

        // Tutkitaan listan viimeistä alkiota ja poistetaan se tietorakenteesta
        Coord top = stack.back();
        stack.pop_back();

        std::shared_ptr<Solmu> top_solmu = solmu_sijainti[(top)];
        // Tutkitaan, onko solmu ja käyty läpi aikaisemmin iteroinnin aikana
        // White - uusi solmu, Gray - Nähty, Black - Läpikäyty
        if(top_solmu->vari == White){
            // Merkitään solmu nähdyksi
            top_solmu->vari = Gray;

            // Jos tutkitava solmu on haluttu päätepiste
            if (top == toxpoint){
                // Alustetaan arvot palautusvectoriin
                int cost = 0;
                std::vector<std::pair<Coord, Cost>> reitti;
                reitti.reserve(solmu_sijainti.size());

                // Tulostetaan reitti eri funktiossa rekursiivisesti
                tulosta_reitti(reitti, cost, fromxpoint, toxpoint); // O(V)
                // Alustetaan kaikki muuttuneet arvot
                alusta(); // O(V)
                return reitti;
            }
            // Laitetaan solmu takaisin listalle
            stack.push_back(top);
            // Tutkitaan kaikki solmun naapurit
            for(auto const& naapuri2 : top_solmu->naapurit){ // O(E)
                std::shared_ptr<Solmu> naapuri = solmu_sijainti[(naapuri2.first)];
                // Jos solmu on uusi
                if(naapuri->vari == White){
                    // Lisätään se TODO-listalle
                    stack.push_back(naapuri2.first);
                    // Ja merkitään solmuun se solmu, mistä tullaan
                    naapuri->edellinen = top_solmu;
                }
            }
        } else {
            // Jos on käyty läpi kaikki naapurit, merkitään solmu mustaksi
            top_solmu->vari = Black;
        }
    }
    // Jos ei löytynyt haluttua päätepistettä, alustetaan muuttuneet arvot
    alusta(); // O(V)
    // Ja palautetaan tyhjä lista
    return {};
    } else {

        // Jos jompaakumpaa solmua ei löydy tietorakenteesta, palautetaan tyhjä lista
        return {};
    }
}

void Datastructures::alusta(){ // O(V)
    // Käydään läpi kaikki solmut
    for (auto const& solmu : solmu_sijainti){
        // Värjätään solmut uusiksi
        solmu.second->vari = White;
        // Nollataan algoritmin käymä polku
        solmu.second->edellinen = nullptr;
        // Merkitään etäisyydet "äärettömiksi"
        solmu.second->etaisyys = 65000;
        // Merkitään arvio loppumatkasta "äärettömäksi"
        //solmu.second->etaisyys_arvio = 65000;
    }
}

void Datastructures::tulosta_reitti(std::vector<std::pair<Coord, Cost>> &reitti, int &cost,
                                    Coord fromxpoint, Coord toxpoint){ // O(V)
    // Kun löytyy aloituspiste
    if (toxpoint == fromxpoint){
        // Listään se ensimmäiseksi polkuun ja lopetetaan rekursio
        reitti.push_back(std::make_pair(fromxpoint, 0));
        return;

    } else {
        // Jos solmu ei ole aloituspiste, mennään polulla edelliseen solmuun
        tulosta_reitti(reitti, cost,
                       fromxpoint,solmu_sijainti[(toxpoint)]->edellinen->sijainti); // O(V)

        // Lisätään kaaren paino
        cost += kaari_hinta(toxpoint, solmu_sijainti[(toxpoint)]->edellinen->sijainti);
        // ja lisätään itse solmu reitille
        reitti.push_back(std::make_pair(toxpoint, cost));
    }
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_least_xpoints(Coord fromxpoint, Coord toxpoint)
{
    // Käytetään funktiossa BFS-algoritmia
    // Tutkitaan löytyykö solmu tietorakenteesta
    if ((solmu_sijainti.find(fromxpoint) != solmu_sijainti.end()) &&
            (solmu_sijainti.find(toxpoint) != solmu_sijainti.end())){

        // Käytetään TODO-listana listaa.
        std::list<Coord> stack;
        // Merkitään aloitussolmu nähdyksi ja lisätään se listalle
        solmu_sijainti[(fromxpoint)]->vari = Gray;
        stack.push_back(fromxpoint);

        while(!stack.empty()){ // O(V)
            // Otetaan listan ensimmäinen alkio ylös ja poistetaan se tietorakenteesta
            Coord top = stack.front();
            stack.pop_front();
            std::shared_ptr<Solmu> top_solmu = solmu_sijainti[(top)];

            // Jos tutkittava alkio on haluttu päätepiste
            if (top == toxpoint){

                // Alustetaan palautusvectorin muuttujat
                int cost = 0;
                std::vector<std::pair<Coord, Cost>> reitti;
                reitti.reserve(solmu_sijainti.size());

                // Tulostetaan reitti rekursiivisesti
                tulosta_reitti(reitti, cost, fromxpoint, toxpoint); // O(V)

                // Alustetaan muuttuneet alkutiedot
                alusta(); // O(V)
                return reitti;

            }

            // Käydään läpi kaikki solmun naapurit
            for (auto const& naapuri : top_solmu->naapurit){ // O(E)
                std::shared_ptr<Solmu> naapurisolmu = solmu_sijainti[(naapuri.first)];
                // Jos solmu on uusi
                if (naapurisolmu->vari == White){
                    // Merkitään se nähdyksi
                    naapurisolmu->vari = Gray;

                    // merkitään solmuun sitä edeltävä solmu
                    naapurisolmu->edellinen = top_solmu;
                    // ja lisätään naapuri TODO-listalle
                    stack.push_back(naapuri.first);
                }
            }
            // Kun on käyty läpi kaikki naapurit, merkitään solmu läpikäydyksi
            top_solmu->vari = Black;

        }
        // Alustetaan muuttuneet muuttujat
        alusta(); // O(V)
        // Jos ei löydetty päätepistettä, palautetaan tyhjä vector
        return {};
    } else {
        // Jos jompaakumpaa solmua ei löydy tietorakenteesta, palautetaan tyhjä lista
        return {};
    }
}

std::vector<std::pair<Coord, Cost>> Datastructures::route_fastest(Coord fromxpoint, Coord toxpoint)
{
    // Käytetään funktiossa Dijkstran algoritmia

    // Tutkitaan löytyykö solmu tietorakenteesta
    if ((solmu_sijainti.find(fromxpoint) != solmu_sijainti.end()) &&
            (solmu_sijainti.find(toxpoint) != solmu_sijainti.end())){

        // Käytetään TODO-listana kekomaista tietorakennetta
        std::priority_queue<std::pair<int, Coord>> stack;
        // Alustetaan etäisyys ja merkitään solmu nähdyksi
        solmu_sijainti[fromxpoint]->etaisyys = 0;
        solmu_sijainti[fromxpoint]->vari = Gray;
        // lisätään aloitusolmu TODO-listaan
        stack.push(std::make_pair(solmu_sijainti[(fromxpoint)]->etaisyys, fromxpoint));

        // Iteroidaan niin kauan, kun TODO-lista ei ole tyhjä
        while(!stack.empty()){ // O(V)

            // Otetaan tietorakenteesta pienin arvo ja poistetaan se tietorakeneesta
            Coord top = stack.top().second;
            stack.pop();
            std::shared_ptr<Solmu> top_solmu = solmu_sijainti[(top)];

            // Jos löydetään haluttu päätepiste
            if (top == toxpoint){
                // Alustetaan palautukseen vaadittavat arvot
                int cost = 0;
                std::vector<std::pair<Coord, Cost>> reitti;
                reitti.reserve(solmu_sijainti.size());

                // TUlostetaan reitti rekursiivisesti
                tulosta_reitti(reitti, cost, fromxpoint, toxpoint); // O(V)
                // Alustetaan muuttuneet arvot
                alusta(); // O(V)
                return reitti;
            }

            // Käydään läpi tarkasteltavat solmun naapurit
            for (auto const& naapuri : top_solmu->naapurit){ // O(E)
                std::shared_ptr<Solmu> naapurisolmu = solmu_sijainti[(naapuri.first)];
                // Jos solmu on uusi
                if (naapurisolmu->vari == White){
                    // meerkitään se nähdyksi ja otetaan ylös se etäisyys lähtösolmuun
                    naapurisolmu->vari = Gray;
                    naapurisolmu->etaisyys = top_solmu->etaisyys
                            + kaari_hinta(naapuri.first, top);

                    // Merkitään reitti edelliseen solmuun
                   naapurisolmu->edellinen = top_solmu;
                    // Lisätään naapuri ja laskettu etäisyys lähtösolmuun TODO-listaan
                    // käytettävä tietorakenne on maksimikeko, joten hinta täytyy kertoa -1.
                    // Tällöin saadaan toteutettua minimikeko.
                    stack.push(std::make_pair(-1*naapurisolmu->etaisyys, naapuri.first));
                }
                // Jos löydetään uusi, halvempi reitti solmuun, merkittän tämä uudeksi reitiksi
                relax_a(top, naapuri.first, toxpoint); // O(1)
            }
            // Kaikki naapurit läpikäytyä, merkitään solmu läpikäydyksi
            top_solmu->vari = Black;

        }
        // Alustetaan muuttuneet arvot
        alusta(); // O(V)
        // Jos ei löydetty päätesolmua, palautetaan tyhjä vector
        return {};
    } else {
        // Jos jompaakumpaa solmua ei löytynyt, palautetaan tyhjä vector
        return {};
    }


}

void Datastructures::relax_a(Coord solmu1, Coord solmu2, Coord /*loppupiste*/){ // O(1)

    // Tutkitaan löytyikö uusi halvin reitti
    if(solmu_sijainti[(solmu2)]->etaisyys >
            solmu_sijainti[(solmu1)]->etaisyys + kaari_hinta(solmu1,solmu2)){
        // Jos löytyi, merkitään tämä uudeksi hinnaksi solmuun
        solmu_sijainti[(solmu2)]->etaisyys = solmu_sijainti[(solmu1)]->etaisyys + kaari_hinta(solmu1, solmu2);

        // sekä uudeksi reitiksi ko. solmuun
        solmu_sijainti[(solmu2)]->edellinen = solmu_sijainti[(solmu1)];
    }
}


std::vector<Coord> Datastructures::route_fibre_cycle(Coord startxpoint)
{
    // Käytetään syklin etsimiseen DFS-algoritmia
    // tallenna structiin sykli? Turhaa?

    // Etsitään solmu tietorakenteesta
    if (solmu_sijainti.find(startxpoint) != solmu_sijainti.end()){

        // Käytetään TODO-listan tietorakenteena list säiliötä
        std::list<Coord> stack;
        // Lisätään aloituspiste listalle
        stack.push_back(startxpoint);

        // Jatketaan iteroitnia niin kauan, kun säiliö ei ole tyhjä
        while(!stack.empty()){ // O(V)

            // Tutkitaan TODO-lista viimeistä arvoa ja poistetaan se tietorakenteesta
            Coord top = stack.back();
            stack.pop_back();
            std::shared_ptr<Solmu> top_solmu = solmu_sijainti[(top)];

            // Jos solmu on uusi
            if(solmu_sijainti[(top)]->vari == White){

                // Merkitään se nähdyksi ja lisätään uudestaan säiliöön
                top_solmu->vari = Gray;
                stack.push_back(top);

                // Käydään läpi kaikki solmun naapurit
                for(auto const& naapuri2 : solmu_sijainti[(top)]->naapurit){ // O(E)
                    std::shared_ptr<Solmu> naapurisolmu = solmu_sijainti[(naapuri2.first)];
                    // Jos solmua ei ole nähty aikaisemmin
                    if(naapurisolmu->vari == White){
                        // Lisätään se TODO-listalle, sekä merkitään edellinen solmu
                        stack.push_back(naapuri2.first);
                        naapurisolmu->edellinen = top_solmu;
                    // Jos löydetään ennen nähty solmu, ollaan löydetty silmukka
                    } else if (naapurisolmu->vari == Gray &&
                               naapuri2.first != top_solmu->edellinen->sijainti){

                        // Alustetaan tulostuksen muuttujat
                        std::vector<Coord> reitti;
                        reitti.reserve(solmu_sijainti.size());

                        // Lisätään ensimmäiseksi arvoksi löydetty silmukan luova solmu
                        reitti.push_back(naapuri2.first);
                        // Tulostetaan loput silmukasta rekursiivisesti
                        tulosta_sykli(reitti, naapurisolmu->sijainti, top); // O(V)
                        // Alustetaan muuttuneet arvot
                        alusta(); // O(V)
                        return reitti;

                    }
                }
            } else {
                // Jos kaikki naapurit on käyty läpi, merkitään solmu läpikäydyksi
                top_solmu->vari = Black;
            }
        }
        // Jos ei löydetty silmukkaa, alustetaan arvot ja palautetaan tyhjä vector
        alusta(); // O(V)
        return {};
    } else {
        // Jos solmua ei löydy tietorakenteesta, palautetaan tyhjä vector
        return {};
    }


}

void Datastructures::tulosta_sykli(std::vector<Coord> &reitti,
                                    Coord fromxpoint, Coord toxpoint){ // O(V)
    // Jos löydettiin haluttu poäätepiste
    if (toxpoint == fromxpoint){
        // Lisätään se reittiin ja lopetetaan rekursio
        reitti.push_back(fromxpoint);
        return;
    } else{
        // Jos ei löydetty päätepistetää, lisätään solmu reittiin
        reitti.push_back(toxpoint);
        // Ja tutkitaan solmua edeltävää solmua
        tulosta_sykli(reitti,
                       fromxpoint,solmu_sijainti[(toxpoint)]->edellinen->sijainti); // O(V)
    }
}

Cost Datastructures::trim_fibre_network()
{
    // Ratkaistaan ongelma Kruskalin algoritmilla

    // Jos kyseessä on jo virittävä puu
    if (is_spanning_tree){
        // palautetaan kaikkien valokuitujen paino
        return spanning_tree_cost;
    }

    // Otetaan ylös kuitujen sekä solmujen määrä
    long unsigned int kuitu_lkm = kuidut.size();
    long unsigned int kuitu_lkm_alussa = kuidut.size();
    int solmu_lkm = solmu_sijainti.size();
    Cost yhteishinta = 0;
    bool virittava_puu = false;

    // Luodaan solmujen määrän verran alkioita vectoriin, jotka alustetaan arvolla -1
    // negatiivinen arvo tarkoittaa, että solmu on puun juuri
    std::vector<int> disjoint_set(solmu_lkm, -1);

    // Luodaan uusi tietorakenne, johon tallennetaan kevyin virittävä puu
    std::vector<std::shared_ptr<Kuitu>> uudet_kuidut{};
    uudet_kuidut.reserve(kuidut.size());

    // Tutkitaan onko jo tiedossa tietorakenteen kuidut suuruusjärjestyksessä
    if (!is_sorted_weight){
        // JOos ei, järjestetään kuidut painojärjestykseen
        std::sort(kuidut.begin(), kuidut.end(),
                  [&](const std::shared_ptr<Kuitu>& lhs, const std::shared_ptr<Kuitu>& rhs)
        {return (lhs->hinta) < (rhs->hinta);}); // O(E log E)

        // Otetaan järjestetyt kuidut ylös ja merkitään tieto tästä
        sorted_weight = kuidut;
        is_sorted_weight = true;
    }

    // Iteroidaan kuitujen lukumäärän verran
    for (long unsigned int i = 0; i < kuitu_lkm; i++){ // O(E)

        // Jos on saavuitettu virittävä puu
        if (virittava_puu){
            // Poistetaan loput kuidut solmujen naapureista molempiin suuntiin
            kuidut[i]->solmu1->naapurit.erase( kuidut[i]->solmu2->sijainti);
            kuidut[i]->solmu2->naapurit.erase( kuidut[i]->solmu1->sijainti);

        } else{

            // Tutkitaan solmujen indeksi
            int s1_indeksi = sorted_weight[i]->solmu1->indeksi;
            int s2_indeksi = sorted_weight[i]->solmu2->indeksi;

            // Ja solmun juuri uudessa tierorakenteessa
            int s1_juuri = etsi_vanhempi(s1_indeksi, disjoint_set); // O(V)
            int s2_juuri = etsi_vanhempi(s2_indeksi, disjoint_set); // O(V)

            // Jos solmuilla on eri juuri, yhdistetään ne
            if (s1_juuri != s2_juuri){

                // Tutkitaan kumpi puu on isompi, yhdistetään pienempi puu tähän
                if (disjoint_set[s1_juuri] <= disjoint_set[s2_juuri]){
                    // Lisätään toisen puun alkiolukumäärä suurempaan
                    disjoint_set[s1_juuri] += disjoint_set[s2_juuri];
                    // merkitään toisen puun juuri osoittamaan suuremman puun juureen
                    disjoint_set[s2_juuri] = s1_juuri;

                } else {
                    disjoint_set[s2_juuri] += disjoint_set[s1_juuri];
                    disjoint_set[s1_juuri] = s2_juuri;

                }

                // Lisätään uudet kuidut tietorakenteeseen ja lasketaan valokuitujen yhteispaino
                uudet_kuidut.push_back(sorted_weight[i]);
                yhteishinta += sorted_weight[i]->hinta;

                // Tutkitaan ollaanko saavuitettu virittävä puu. Solmuun liitetty negatiivinen arvo
                // kuvaa juuressa olevian alkioiden määrää, jota verrataan alussa ylösotettuun lukumäärään.
                if (-1* disjoint_set[s1_juuri] == solmu_lkm ||
                        -1* disjoint_set[s2_juuri] == solmu_lkm){
                    // merkitään puu virittäväksi puuksi
                    virittava_puu = true;
                }
            } else {
                // Jos solmuilla oli sama juuri, poistetaan nykyinen valokuitu solmujen välistä
                kuidut[i]->solmu1->naapurit.erase( kuidut[i]->solmu2->sijainti);
                kuidut[i]->solmu2->naapurit.erase( kuidut[i]->solmu1->sijainti);
            }
        }
    }

    // Tyhjennetään vanha kuitu-tietorakenne
    kuidut.clear(); // O(E)
    // Ja merkitään kuiduiksi uusi kevennetty rakenne
    kuidut = uudet_kuidut;

    // Jos poistettiin kuitu, on järkevämpää järjestää kuidut uudestaan kutsuttaessa all_fibres
    // funktiota. On melko tehotonta etsiä jokainen jäljelle jäänyt kaari mahdollisesti molemmista
    // tietorakenteista
    if (kuitu_lkm_alussa != kuidut.size()){
        poistettu_kuituja = true;
    }

    // Merkitään muuttuneet liput
    is_sorted_fibres = false;
    is_sorted_xpoints = false;

    is_spanning_tree = true;
    // Otetaan talteen puun yhteispaino
    spanning_tree_cost = yhteishinta;
    return yhteishinta;
}

int Datastructures::etsi_vanhempi(int &solmu, std::vector<int> &disjoint_set){ // O(V)
    // Tutkitaan löytyikö puun juuri
    if (disjoint_set[solmu] <= -1){
        return solmu;
    } else {
        // Jos solmu ei ollut puu, tutkitaan solmun vanhempi
        return etsi_vanhempi(disjoint_set[solmu], disjoint_set);
    }
}

bool Datastructures::isEdge(Coord solmu1, Coord solmu2){ // O(log V)
    // Tutkitaan löytyykö solmun tietuesta haluttu toinen solmu
    if (solmu_sijainti[(solmu1)]->naapurit.find(solmu2) != solmu_sijainti[(solmu1)]->naapurit.end()){
        return true;
    } else {
        return false;
    }
}

int Datastructures::kaari_hinta(Coord solmu1, Coord solmu2){ // O(1)
    // Palauttaa kaaren painon
    return solmu_sijainti[(solmu1)]->naapurit[(solmu2)];
}
