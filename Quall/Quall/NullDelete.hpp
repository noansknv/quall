/**
 * Bardzo sprytne narzedzie do boost::shared_ptr.
 * Czasem okazuje sie, ze to nie smartpointer powinien byc odpowiedzialny za usuniecie elementu,
 * lecz sam ogre. Przykladem jest Application::ogWorld, bez tego sypie wyjatkami.
 *
 * @author marcin.kanclerz
 */
struct NullDelete
{
  void operator() (void*) const
  {/* in Ogre we trust */}
};