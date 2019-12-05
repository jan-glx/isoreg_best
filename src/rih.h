#ifndef RIH_HPP
#define RIH_HPP

#include <vector>
#include <algorithm>
#include <cassert>

namespace RIH {

namespace TiesMethod {
	template <class T1, class T2, class T3>
	void first(T1 const X, T2 const order, T3 &ranks) {
		typename decltype(order)::size_type i = 0;
		for (; i < order.size(); ++i) {
			ranks[order[i]] = i;
		}
	};

	template <class T1, class T2, class T3>
	void max(T1 const X, T2 const order, T3 &ranks) {
		typename decltype(order)::size_type i = 0;
		for (; i < order.size(); ++i) {
			ranks[order[i]] = i;
			for (auto j = i; j-- > 0 && X[order[j]] == X[order[i]];) {
				ranks[order[j]] = i;
			}
		}
	};

	template <class T1, class T2, class T3>
	void min(T1 const X, T2 const order, T3 &ranks) {
		for (auto i = order.size(); i > 0;) {
			--i;
			for (auto j = i; j < order.size() && X[order[j]] == X[order[i]]; ++j) {
				ranks[order[j]] = i;
			}
		}
	};

	template <class T1, class T2, class T3>
	void average(T1 const X, T2 const order, T3 &ranks) {
		for (auto i = order.size(); i > 0;) {
			--i;
			decltype(i) n = 0;
			auto j = i + 1;
			for (; j < order.size() && X[order[j]] == X[order[i]]; ++j) {
				++n;
			}
			double val = i + n / 2.0;
			for (; j > i;) {
				--j;
				ranks[order[j]] = val;
			}
		}
	};
}

template<class T>
std::pair<std::vector<typename T::size_type>, std::vector<double>>
order_and_ranks(T const X, void (* const tiesMethod)(T, std::vector<typename T::size_type>, std::vector<double>&)) {
	auto n = X.size();

	auto ranks = std::vector<double>(n);
	auto order = std::vector<typename T::size_type>(n);
	typename decltype(order)::size_type i = 0;
	std::generate(order.begin(), order.end(), [&] { return i++; });

	std::sort(order.begin(), order.end(), [&X] (typename T::size_type a, typename T::size_type b) { return X[a] < X[b]; });

	tiesMethod(X, order, ranks);

	return { order, ranks };
}

template <class T>
std::vector<std::size_t>
rank_in_brute(std::vector<std::vector<T>> const X, std::vector<std::vector<T>> const Q, typename std::vector<T>::size_type const p) {
	auto nX = X.size();
	auto ret = std::vector<std::size_t>(Q.size());
	auto itRet = ret.begin();
	for (auto i = Q.begin(); i != Q.end(); ++i) {
		std::vector<bool> b(nX, true);
		for (auto j = decltype(p){0}; j < p; ++j) {
			auto itB = b.begin();
			for (auto itX = X.begin(); itX != X.end(); ++itX) {
				*itB = *itB && ((*i)[j] >= (*itX)[j]);
				++itB;
			}
		}
		*itRet++ = std::count(b.begin(), b.end(), true);
	}
	return ret;
}

template <class T>
static std::vector<std::size_t>
rank_in_hybrid_(std::vector<std::vector<T>> X, std::vector<bool> const isQ, typename std::vector<T>::size_type const p, std::size_t const nsw = 100) {
	auto n = X.size();
	assert(X.size() == isQ.size());
	assert(p > 0);
	assert(X.size() == 0 || X[0].size() == p);

	// X[, 1]
	auto X0 = std::vector<T>(n);
	std::transform(X.begin(), X.end(), X0.begin(), [] (std::vector<T> const x) { return x.front(); });

	auto nQ = std::count(isQ.begin(), isQ.end(), true);

	if (p == 1) {
		// ranks<-rank(X[,1],ties.method ="max")
		// orders<-order(X[,1])
		auto oar = order_and_ranks(X0, TiesMethod::max);
		assert(oar.first.size() == n);

		// return(cumsum(!isQ[orders])[ranks[isQ]])
		typename decltype(oar.first)::size_type sum = 0;
		auto cumsum = std::vector<decltype(sum)>(n);
		for (decltype(n) i = 0; i < n; ++i) {
			if (!isQ[oar.first[i]]) ++sum;
			cumsum[i] = sum;
		}
		assert(sum == n - nQ);

		auto ret = std::vector<std::size_t>(nQ);
		typename decltype(ret)::size_type k = 0;
		for (typename decltype(isQ)::size_type i = 0; i < n; ++i) {
			if (isQ[i]) {
				assert(k < nQ);
				ret[k++] = cumsum[oar.second[i]];
			}
		}
		assert(k == nQ);

		return ret;
	}

	if (n <= nsw) {
		auto onlyX = decltype(X)(n - nQ), Q = decltype(X)(nQ);
		auto itX = X.begin();
		auto itIsQ = isQ.begin();
		auto itOnlyX = onlyX.begin();
		auto itQ = Q.begin();
		while (itX != X.end()) {
			(*itIsQ++ ? *itQ++ : *itOnlyX++) = *itX++;
		}

		return rank_in_brute(onlyX, Q, p);
	}

	// if (all(X[,1]==X[1,1])) return(rank_in_hybrid_(X[,-1,drop=FALSE],isQ, nsw))
	bool all_equal = true;
	for (auto it = X0.begin() + 1; it != X0.end(); it++) {
	  if (*it !=  X0.front()) {
	    all_equal = false;
	    break;
	  }
	}
	if (all_equal) {
		assert(X.size() == 0 || X.front().size() > 0);
		for (auto &x: X) {
			x.erase(x.begin()); // maybe copy instead and make X const?
		}
		return rank_in_hybrid_(X, isQ, p - 1, nsw);
	}

	// inA <-rank(X[,1],ties.method =  "average")<=n/2
	auto inA = std::vector<bool>(n);
	typename decltype(inA)::size_type nInA = 0;
	{
	 	auto ranks = order_and_ranks(X0, TiesMethod::average).second;
		for (typename decltype(ranks)::size_type i = 0; i < ranks.size(); ++i) {
			if (ranks[i] <= n / 2.0 - 1) {
				inA[i] = true;
				++nInA;
			} else {
				inA[i] = false;
			}
		}
	}

	auto XInA = decltype(X)(nInA), XNotInA = decltype(X)(n - nInA);
	auto isQInA = decltype(isQ)(nInA), isQNotInA = decltype(isQ)(n - nInA);
	typename decltype(X)::size_type iInA = 0, iNotInA = 0;
	for (decltype(n) i = 0; i < n; ++i) {
		if (inA[i]) {
			assert(iInA < nInA);
			XInA[iInA] = X[i];
			isQInA[iInA] = isQ[i];
			++iInA;
		} else {
			assert(iNotInA < n - nInA);
			XNotInA[iNotInA] = X[i];
			isQNotInA[iNotInA] = isQ[i];
			++iNotInA;
		}
	}
	assert(iInA == nInA);
	assert(iNotInA == n - nInA);

	// ECDF_A <- rank_in_hybrid_(X[inA, ,drop=FALSE], isQ[ inA], nsw)
	// ECDF_B <- rank_in_hybrid_(X[!inA,,drop=FALSE], isQ[!inA], nsw)
	auto ECDF_A = rank_in_hybrid_(XInA, isQInA, p, nsw);
	auto ECDF_B = rank_in_hybrid_(XNotInA, isQNotInA, p, nsw);
	assert(ECDF_B.size() == std::count(isQNotInA.begin(), isQNotInA.end(), true));

	// inI <- xor(isQ,inA)
	// ranks <- rank_in_hybrid_(X[inI,-1,drop=FALSE], isQ[inI], nsw)
	auto XInI = decltype(X)();
	auto isQInI = decltype(isQ)();
	std::size_t tmp = 0;
	for (decltype(n) i = 0; i < n; ++i) {
		if (isQ[i] != inA[i]) {
			// only these points (X that are not Q in A and Q from B) need further investigation
			XInI.emplace_back(X[i].begin() + 1, X[i].end());
			isQInI.push_back(isQ[i]);
			if (isQ[i]) ++tmp;
		}
	}

	auto ranks = rank_in_hybrid_(XInI, isQInI, p - 1, nsw);
	assert(tmp == ranks.size());
	assert(ECDF_B.size() == ranks.size());
	std::transform(ECDF_B.begin(), ECDF_B.end(), ranks.begin(), ECDF_B.begin(), std::plus<std::size_t>()); // ECDF_B += ranks;

	// ECDF_AB <- rep(NA_integer_,nQ)
	// ECDF_AB[inA[isQ]] <- ECDF_A
	// ECDF_AB[!inA[isQ]] <- ECDF_B
	auto ECDF_AB = std::vector<std::size_t>(nQ);

	auto itA = ECDF_A.begin();
	auto itB = ECDF_B.begin();
	decltype(nInA) j = 0;
	for (decltype(nInA) i = 0; i < n; ++i) {
		if (isQ[i]) {
			assert(j < nQ);
			ECDF_AB[j] = inA[i] ? *itA++ : *itB++;
			++j;
		}
	}
	assert(j == nQ);

	return ECDF_AB;
}

template <class T>
std::vector<std::size_t>
rank_in_hybrid(std::vector<std::vector<T>> X, std::vector<std::vector<T>> const Q, std::size_t const nsw = 100) {
	std::vector<bool> isQ(X.size(), false);
	X.insert(X.end(), Q.begin(), Q.end()); // append Q to X
	isQ.resize(X.size(), true);

	return rank_in_hybrid_(X, isQ, X.front().size(), nsw);
}

}
#endif // #ifndef RIH_HPP
