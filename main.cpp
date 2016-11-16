#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<map>
#include<set>
#include<unordered_map>

#define db(a) cout << #a << " = " << a << endl;

using namespace std;

string get_label_feature(int index) {
	/*switch (index)
	{
		case 0: return "outlook";
		case 1: return "temperature";
		case 2: return "humidity";
		case 3: return "wind";
	}*/
	switch (index)
	{
		case 0 : return "buying";
		case 1 : return "maint";
		case 2 : return "doors";
		case 3 : return "persons";
		case 4 : return "lug_boot";
		case 5 : return "safety";
	}
	return "";
}
int main() {
	freopen("car.data", "r", stdin);
	//freopen("car.out", "w", stdout);
	
	string linea, feature;
	vector<string> vline;
	unordered_map< string, unordered_map<string, unordered_map<string, pair <int, double> > > > map_class_features;
	unordered_map<string, pair<int, double> > count_per_class;

	int i = 0, total = 0;
	while (cin >> linea) {
		//cout << "antes: " << linea << endl;
		transform(linea.begin(), linea.end(), linea.begin(),
			[](char michar) -> char {
				if (michar == ',') return ' ';
				return michar;
		});
		
		stringstream ss(linea);
		vline.clear();

		while (ss >> feature) {
			vline.push_back(feature);
		}
		
		string clase = vline[vline.size() - 1];
		count_per_class[clase].first++;
		total++;
		for (size_t i = 0; i < vline.size() - 1; i++) {
			map_class_features[clase][get_label_feature(i)][vline[i]].first++;	
		}

		//if (++i == 5) break;
		
	}
	unordered_map<string, pair<int, double> >::iterator it;

	cout << "clases distintas :" << endl;
	cout << count_per_class.size() << endl;

	
	
	unordered_map< string, unordered_map<string, pair<int, double> > >::iterator itt;
	unordered_map<string, pair<int, double> > ::iterator ittt;

	// TRAINING FASE
	// calculamos las probabilidades de cada característica según sea la clase a la cuál pertenece
	cout << "total: " << total << endl;
	std::cout << "detalles:" << endl;
	for (it = count_per_class.begin(); it != count_per_class.end(); it++) {
		it->second.second = it->second.first * 1. / total;
		cout << "class name: " << it->first << " total : " << it->second.first << " - prob: " << it->second.second << endl;
		cout << "============================================" << endl;
		for (itt = map_class_features[it->first].begin(); itt != map_class_features[it->first].end(); itt++) {
			cout << "feature: " << itt->first << endl;
			for (ittt = itt->second.begin(); ittt != itt->second.end(); ittt++) {
				ittt->second.second = ittt->second.first * 1. / it->second.first;
				cout << "name = " << ittt->first << " count = " << ittt->second.first << " prob:" << ittt->second.second << endl;
			}
		}
		cout << "============================================" << endl;
	}
	
	//fclose(stdout);
	ifstream ifs ("car-prueba.data", ifstream::in);
	// TESTING FASE
	puts("FASE DE TESTING");
	vector< pair<string, double> > clases;
	total = 0;
	int match = 0;
	while (ifs >> linea) {
		total++;
		clases.clear();
		//cout << "linea: " << linea << endl;
		transform(linea.begin(), linea.end(), linea.begin(),
			[](char michar) -> char {
			if (michar == ',') return ' ';
			return michar;
		});

		stringstream ss(linea);
		vline.clear();

		while (ss >> feature) {
			cout << "feature : " << feature << endl;
			vline.push_back(feature);
		}
		//puts("_______________________________________________________________");
		for (it = count_per_class.begin(); it != count_per_class.end(); it++) {
			/*cout << "para la clase : " << it->first << endl;
			cout << "la prob. es : " << it->second.second << endl;*/
			
			double PFiClass = it->second.second;
			int i = 0, times = 0;
			for (itt = map_class_features[it->first].begin(); itt != map_class_features[it->first].end(); itt++) {
				//cout << "feature: " << itt->first << endl;
				bool found = false;
				for (ittt = itt->second.begin(); ittt != itt->second.end() && !found && PFiClass != 0; ittt++) {
					/*db(vline[i])
					db(ittt->first);
					puts("------------------------------------------------");*/
					if (ittt->first == vline[i])
						PFiClass *= ittt->second.second, times++, found = true;
				}
				i++;
				if (!found) PFiClass = 0;
				//puts("====================================");
			}
			//cout << "TIMES : " << times << endl;

			clases.push_back(make_pair(it->first, PFiClass));
		}
		double best = -1;
		string best_class = "";
		for (size_t i = 0; i < clases.size(); i++)
		{
			cout << "clase : " << clases[i].first << " prob : " << clases[i].second << endl;
			if (best < clases[i].second) {
				best_class = clases[i].first;
				best = clases[i].second;
			}
		}
		
		//si predice correctamente entonces es un match (para verificar la precisión de naive bayes)
		if (best_class == vline[vline.size() - 1]) match++;

		cout << "the best Class is :" << best_class << endl;
		cout << "   with the value : " << best << endl;
		cout << "==============================================" << endl;
	}
	
	db(total);
	db(match);
	cout << "La precision de Naive Bayes es : " << match * 1. / total << endl;


	return 0;
}